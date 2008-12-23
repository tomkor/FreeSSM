/*
 * SSMprotocol.cpp - Application Layer for the new Subaru SSM protocol
 *
 * Copyright © 2008 Comer352l
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SSMprotocol.h"




SSMprotocol::SSMprotocol(serialCOM *port, CUtype_dt CU, QString language)
{
	_port = port;
	_CU = CU;
	_language = language;
	_SSMPcom = NULL;
	_state = state_needSetup;
	resetCUdata();
}



SSMprotocol::~SSMprotocol()
{
	resetCUdata();
}



SSMprotocol::CUtype_dt SSMprotocol::CUtype()
{
	return _CU;
}



SSMprotocol::state_dt SSMprotocol::state()
{
	return _state;
}



void SSMprotocol::resetCUdata()
{
	unsigned int k = 0;
	bool ok = false;
	unsigned int dataadr = 0x61;
	char currentdatabyte = '\x0';
	// RESET COMMUNICATION:
	if (_SSMPcom != NULL)
	{
		// Disconnect communication error and data signals:
		disconnect( _SSMPcom, SIGNAL( commError() ), this, SIGNAL( commError() ) );
		disconnect( _SSMPcom, SIGNAL( commError() ), this, SLOT( resetCUdata() ) );
		disconnect( _SSMPcom, SIGNAL( recievedData(QByteArray, int) ),
			    this, SLOT( processDCsRawdata(QByteArray, int) ) );
		disconnect( _SSMPcom, SIGNAL( recievedData(QByteArray, int) ),
			    this, SLOT( processMBSWrawData(QByteArray, int) ) );
		// Try to stop active communication processes:
		ok = _SSMPcom->stopCommunication();
		if (ok && (_state == state_ActTesting))
		{
			ok = _SSMPcom->readMultipleDatabytes(0x0, &dataadr, 1, &currentdatabyte);
			if (ok)
			{
				// Check if test mode is active:
				if (currentdatabyte == (currentdatabyte | 0x20))
				{
					// Stop all actuator tests:
					for (k=0; k<_nrofAllActByteAddr; k++)
					{
						ok =_SSMPcom->writeDatabyte(_allActByteAddr[k], 0x00);
						if (!ok) break;
					}
					_state = state_needSetup;	// MUST BE DONE AFTER ALL CALLS OF MEMBER-FUNCTIONS AND BEFORE EMITTING SIGNALS
					if (ok)
						emit stoppedActuatorTest();
				}
			}
		}
		// NOTE: DO NOT CALL stopCommOperation() or any other communicating functions here because of possible recursions !
		_state = state_needSetup;	// MUST BE DONE AFTER ALL CALLS OF MEMBER-FUNCTIONS AND BEFORE EMITTING SIGNALS
		delete _SSMPcom;
		_SSMPcom = NULL;
		// Emit stoppedXXX()-signals (_SSMPcom has been deletet, so we are sure they have finished):
		if (_state == state_MBSWreading)
		{
			emit stoppedMBSWreading();
		}
		else if (_state == state_DCreading)
		{
			emit stoppedDCreading();
		}
	}
	else
		_state = state_needSetup;	// MUST BE DONE AFTER ALL CALLS OF MEMBER-FUNCTIONS AND BEFORE EMITTING SIGNALS
	// RESET ECU RAW DATA:
	_SYS_ID[0] = 0;
	_SYS_ID[1] = 0;
	_SYS_ID[2] = 0;
	_ROM_ID[0] = 0;
	_ROM_ID[1] = 0;
	_ROM_ID[2] = 0;
	_ROM_ID[3] = 0;
	_ROM_ID[4] = 0;
	for (k=0; k<96; k++) _flagbytes[k] = 0;
	_nrofflagbytes = 0;
	// *** RESET BASIC DATA ***:
	// Reset DC-data:
	for (k=0; k<SSMP_MAX_DTCADDR; k++) _temporaryDTCsAddr[k] = 0;
	for (k=0; k<SSMP_MAX_DTCADDR; k++) _memorizedDTCsAddr[k] = 0;
	_nrofDTCsAddr = 0;
	for (k=0; k<SSMP_MAX_CCCCADDR; k++) _latestCCCCsAddr[k] = 0;
	_nrofLatestCCCCsAddr = 0;
	for (k=0; k<SSMP_MAX_CCCCADDR; k++) _memorizedCCCCsAddr[k] = 0;
	_nrofMemorizedCCCCsAddr = 0;
	// Reset MB/SW-data:
	_supportedMBs.clear();
	_supportedSWs.clear();
	// Reset adjustments-data:
	_adjustments.clear();
	// Reset actuator-test-data:
	for (k=0; k<SSMP_MAX_ACTTESTS; k++)
	{
		_actuators[k].title = "";
		_actuators[k].byteadr = 0;
		_actuators[k].bitadr = 0;
	}
	_nrofActuators = 0;
	for (k=0; k<SSMP_MAX_ACTBYTEADDR; k++) _allActByteAddr[k] = 0;
	_nrofAllActByteAddr = 0;
	// *** Reset selection data ***:
	_selectedDCgroups = noDCs_DCgroup;
	_ignoreDCheckStateOnDCreading = false;
	_MBSWmetaList.clear();
	for (k=0; k<(SSMP_MAX_MBSW); k++) _selMBsSWaAddr[k] = 0;
	_selMBsSWsAddrLen = 0;
	_selectedActuatorTestIndex = 255; // index ! => 0=first actuator !
	// *** Reset Diagnostic Code Definitions ***:
	_DTC_rawDefs.clear();
	_CC_rawDefs.clear();
}



bool SSMprotocol::setupCUdata()
{
	char CUaddress = 0;
	bool ATsup = false;
	bool CCsup = false;
	bool obdDTCs = false;
	bool ok = false;
	QStringList allOBDDTCrawDefs;
	// Reset:
	resetCUdata();
	// Create SSMPcommunication-object:
	if (_CU == ECU)
	{
		CUaddress = '\x10';
	}
	else if (_CU == TCU)
	{
		CUaddress = '\x18';
	}
	_SSMPcom = new SSMPcommunication(_port, CUaddress);
	// Get control unit data:
	if (!_SSMPcom->getCUdata(_SYS_ID, _ROM_ID, _flagbytes, &_nrofflagbytes))
		 return false;
	_state = state_normal;
	// Connect communication error signals from SSMPcommunication:
	connect( _SSMPcom, SIGNAL( commError() ), this, SIGNAL( commError() ) );
	connect( _SSMPcom, SIGNAL( commError() ), this, SLOT( resetCUdata() ) );
	// Get memory addresses of diagnostic codes:
	setupDTCaddresses(_flagbytes, _nrofflagbytes, _temporaryDTCsAddr, _memorizedDTCsAddr, &_nrofDTCsAddr);
	hasIntegratedCC(&CCsup);
	if ( CCsup )	// not necessary, because function checks this, too...
		setupCCCCaddresses(_latestCCCCsAddr, &_nrofLatestCCCCsAddr, _memorizedCCCCsAddr, &_nrofMemorizedCCCCsAddr);
	// Get and save the DC-definitions we need for the used DC addresses:
	obdDTCs = (_flagbytes[29] != (_flagbytes[29] | 0x80));
	if (_language == "de")
	{
		SSMprotocol_def_de rawdefs_de;
		if (obdDTCs)
			allOBDDTCrawDefs = rawdefs_de.OBDDTCrawDefs();
		else
			_DTC_rawDefs = rawdefs_de.SUBDTCrawDefs();
		if (CCsup)
			_CC_rawDefs = rawdefs_de.CCCCrawDefs();
	}
	else
	{
		SSMprotocol_def_en rawdefs_en;
		if (obdDTCs)
			allOBDDTCrawDefs = rawdefs_en.OBDDTCrawDefs();
		else
			_DTC_rawDefs = rawdefs_en.SUBDTCrawDefs();
		if (CCsup)
			_CC_rawDefs = rawdefs_en.CCCCrawDefs();
	}
	for (unsigned int k=0; k<_nrofDTCsAddr; k++)
	{
		for (int m=0; m<allOBDDTCrawDefs.size(); m++)
		{
			if (_temporaryDTCsAddr[k] == allOBDDTCrawDefs.at(m).section(';', 0, 0).toUInt(&ok, 16))
				_DTC_rawDefs.append( allOBDDTCrawDefs.at(m) );
		}
	}
	// Get supported MBs and SWs:
	setupSupportedMBs(_CU, _language, _flagbytes, _nrofflagbytes, &_supportedMBs);
	setupSupportedSWs(_CU, _language, _flagbytes, _nrofflagbytes, &_supportedSWs);
	// Get supported Adaptions:
	setupAdjustmentsData(_CU, _language, _flagbytes, _nrofflagbytes, &_adjustments);
	// Get actuator test data:
	hasActuatorTests(&ATsup);
	if (ATsup)
		setupActuatorTestData(_language, _actuators, &_nrofActuators, _allActByteAddr, &_nrofAllActByteAddr);
	return true;
}



bool SSMprotocol::getSysID(QString *SYS_ID)
{
	if (_state == state_needSetup) return false;
	SYS_ID->clear();
	StrToHexstr(_SYS_ID, 3, SYS_ID);
	return true;
}



bool SSMprotocol::getROMID(QString *ROM_ID)
{
	if (_state == state_needSetup) return false;
	ROM_ID->clear();
	StrToHexstr(_ROM_ID, 5, ROM_ID);
	return true;
}



bool SSMprotocol::getSystemDescription(QString *sysdescription)
{
	if (_state == state_needSetup) return false;
	if (_CU == ECU)
	{
		return getSysDescriptionBySysID( SSMprotocol_ID::ECU_sysID, _SYS_ID, sysdescription );
	}
	else if (_CU == TCU)
	{
		return getSysDescriptionBySysID( SSMprotocol_ID::TCU_sysID, _SYS_ID, sysdescription );
	}
	return false;
}



bool SSMprotocol::hasOBD2(bool *OBD2)
{
	if (_state == state_needSetup) return false;
	if ((_flagbytes[29] != (_flagbytes[29] | 0x80)) && (_flagbytes[28] != (_flagbytes[28] | 0x02)))
		*OBD2 = true;
	else
		*OBD2 = false;
	return true;
}



bool SSMprotocol::hasVINsupport(bool *VINsup)
{
	if (_state == state_needSetup) return false;
	if ((_CU==ECU) && (_nrofflagbytes > 32))
	{
		if (_flagbytes[36] == (_flagbytes[36] | 0x01))
			*VINsup = true;
		else
			*VINsup = false;
	}
	else
		*VINsup = false;
	return true;
}



bool SSMprotocol::hasImmobilizer(bool *ImmoSup)
{
	if (_state == state_needSetup) return false;
	if (_CU==ECU && (_flagbytes[11] == (_flagbytes[11] | 0x20)))
	{
		if (_flagbytes[28] == (_flagbytes[28] | 0x10))
			*ImmoSup = true;
		else
			*ImmoSup = false;
	}
	else
		*ImmoSup = false;

	return true;
}



bool SSMprotocol::hasIntegratedCC(bool *CCsup)
{
	if (_state == state_needSetup) return false;
	if ((_CU==ECU) && (_nrofflagbytes > 32))
	{
		if (_flagbytes[39] == (_flagbytes[39] | 0x01))
			*CCsup = true;
		else
			*CCsup = false;
	}
	else
		*CCsup = false;
	return true;
}



bool SSMprotocol::hasIntegratedCCmemorizedCodes(bool *CCmemCCsup)
{
	if (_state == state_needSetup) return false;
	if ((_CU==ECU) && (_nrofflagbytes > 32))
	{
		if ((_flagbytes[41] == (_flagbytes[41] | 0x04)) && (_flagbytes[39] == (_flagbytes[39] | 0x01)))
			*CCmemCCsup = true;
		else
			*CCmemCCsup = false;
	}
	else
		*CCmemCCsup = false;
	return true;
}



bool SSMprotocol::hasClearMemory2(bool *CM2sup)
{
	if (_state == state_needSetup) return false;
	if ((_CU==TCU) && (_nrofflagbytes > 32))
	{
		if (_flagbytes[39] == (_flagbytes[39] | 0x02))
			*CM2sup = true;
		else
			*CM2sup = false;
	}
	else
		*CM2sup = false;
	return true;
}



bool SSMprotocol::hasTestMode(bool *TMsup)
{
	if (_state == state_needSetup) return false;
	if ((_CU==ECU) && (_flagbytes[11] == (_flagbytes[11] | 0x20)))
		*TMsup = true;
	else 
		*TMsup = false;
	return true;
}



bool SSMprotocol::hasActuatorTests(bool *ATsup)
{
	bool TMsup = false;
	if (!hasTestMode(&TMsup))	// includes check of status
		return false;
	*ATsup = false;
	if ((_CU==ECU) && TMsup)
	{
		if (_flagbytes[28] == (_flagbytes[28] | 0x40))
		{
			if (_flagbytes[0] == (_flagbytes[0] | 0x01))
				*ATsup = true;
		}
	}
	return true;
}



void SSMprotocol::setupDTCaddresses(char flagbytes[96], unsigned char nrofflagbytes, unsigned int *temporaryDTCsAddr, unsigned int *memorizedDTCsAddr, unsigned char *nrofDTCsAddr)
{
	*nrofDTCsAddr = 0;
	unsigned int addr = 0;
	if (flagbytes[29] == (flagbytes[29] | 0x80))
	{
		for (addr=0x8E; addr<=0x98; addr++)
		{
			temporaryDTCsAddr[*nrofDTCsAddr] = addr;
			memorizedDTCsAddr[*nrofDTCsAddr] = addr + 22;
			(*nrofDTCsAddr)++;
		}
		return;
	}
	else if ((flagbytes[29] == (flagbytes[29] | 0x10)) || (flagbytes[29] == (flagbytes[29] | 0x40)))
	{
		for (addr=0x8E; addr<=0xAD; addr++)
		{
			temporaryDTCsAddr[*nrofDTCsAddr] = addr;
			memorizedDTCsAddr[*nrofDTCsAddr] = addr + 32;
			(*nrofDTCsAddr)++;
		}
	}
	if (flagbytes[28] == (flagbytes[28] | 0x01))
	{
		for (addr=0xF0; addr<=0xF3; addr++)
		{
			temporaryDTCsAddr[*nrofDTCsAddr] = addr;
			memorizedDTCsAddr[*nrofDTCsAddr] = addr + 4;
			(*nrofDTCsAddr)++;
		}
	}
	if (nrofflagbytes > 32)
	{
		if (flagbytes[39] == (flagbytes[39] | 0x80))
		{
			for (addr=0x123; addr<=0x12A; addr++)
			{
				temporaryDTCsAddr[*nrofDTCsAddr] = addr;
				memorizedDTCsAddr[*nrofDTCsAddr] = addr + 8;
				(*nrofDTCsAddr)++;
			}
		}
		if (flagbytes[39] == (flagbytes[39] | 0x40))
		{
			for (addr=0x150; addr<=0x154; addr++)
			{
				temporaryDTCsAddr[*nrofDTCsAddr] = addr;
				memorizedDTCsAddr[*nrofDTCsAddr] = addr + 5;
				(*nrofDTCsAddr)++;
			}
		}
		if (flagbytes[39] == (flagbytes[39] | 0x20))
		{
			for (addr=0x160; addr<=0x164; addr++)
			{
				temporaryDTCsAddr[*nrofDTCsAddr] = addr;
				memorizedDTCsAddr[*nrofDTCsAddr] = addr + 5;
				(*nrofDTCsAddr)++;
			}
		}
		if (flagbytes[39] == (flagbytes[39] | 0x10))
		{
			for (addr=0x174; addr<=0x17A; addr++)
			{
				temporaryDTCsAddr[*nrofDTCsAddr] = addr;
				memorizedDTCsAddr[*nrofDTCsAddr] = addr + 7;
				(*nrofDTCsAddr)++;
			}
		}
	}
}



void SSMprotocol::setupCCCCaddresses(unsigned int *latestCCCCsAddr, unsigned char *nrofLatestCCCCsAddr, unsigned int *memorizedCCCCsAddr, unsigned char *nrofMemorizedCCCCsAddr)
{
	bool supported = false;
	*nrofLatestCCCCsAddr = 0;
	*nrofMemorizedCCCCsAddr = 0;
	unsigned int addr = 0;
	// Check if CU is equipped with CC:
	hasIntegratedCC(&supported);
	if (!supported) return;
	// Put addresses on the list:
	for (addr=0x133; addr<=0x136; addr++)
	{
		latestCCCCsAddr[*nrofLatestCCCCsAddr] = addr;
		(*nrofLatestCCCCsAddr)++;
	}
	// Check if CC supports memorized Codes:
	hasIntegratedCCmemorizedCodes(&supported);
	if (supported)
	{
		// Put addresses of memorized codes on the list:
		for (addr=0x137; addr<=0x13A; addr++)
		{
			memorizedCCCCsAddr[*nrofMemorizedCCCCsAddr] = addr;
			(*nrofMemorizedCCCCsAddr)++;
		}
	}
}



void SSMprotocol::setupSupportedMBs(CUtype_dt CU, QString language, char flagbytes[96], unsigned char nrofflagbytes, std::vector<mb_intl_dt> *supportedMBs)
{
	QString mbdefline;
	QString tmpstr;
	int tmpbytenr=0;
	int tmpbitnr=0;
	int tmpCUsupported=0;
	int tmpprecision=0;
	mb_intl_dt tmpMB;
	bool ok;
	int k;

	supportedMBs->clear();
	// Select definitions depending on language:
	QStringList mbrawdata;
	if (language == "de")
	{
		SSMprotocol_def_de rawdefs_de;
		mbrawdata = rawdefs_de.MBrawDefs();
	}
	else
	{
		SSMprotocol_def_en rawdefs_en;
		mbrawdata = rawdefs_en.MBrawDefs();
	}
	// Assort list of supported MBs:
	for (k=0; k<mbrawdata.size(); k++)
	{
		// Get flagbyte address definition:
		mbdefline = mbrawdata.at(k);
		tmpstr = mbdefline.section(';', 0, 0);
		tmpbytenr = tmpstr.toInt(&ok, 10);
		// Check if flagbyte address is defined for our CU:
		if (ok && (tmpbytenr <= nrofflagbytes))
		{
			// Get flagbit definition:
			tmpstr = mbdefline.section(';', 1, 1);
			tmpbitnr = tmpstr.toInt();
			// Check if CU supports this MB (if flagbit is set):
			if ((tmpbitnr < 1) || (tmpbitnr > 8))
				ok = false;
			if ((ok) && (flagbytes[tmpbytenr-1] == (flagbytes[tmpbytenr-1] | static_cast<unsigned char>(pow(2, (tmpbitnr-1))))))
			{
				// Check if MB is intended for this CU type:
				tmpCUsupported = 0;
				if (CU == (CU | ECU))	// if MB is intended for ECUs
				{
					tmpstr = mbdefline.section(';', 2, 2);
					tmpCUsupported = tmpstr.toInt();
				}
				if ((!(tmpCUsupported == 1)) && (CU == (CU | TCU)))	// if MB is intended for TCUs (and not for ECUs)
				{
					tmpstr = mbdefline.section(';', 3, 3);
					tmpCUsupported = tmpstr.toInt();
				}
				if (tmpCUsupported == 1)
				{
					// Get memory address (low) definition:
					tmpstr = mbdefline.section(';', 4, 4);
					tmpMB.adr_low = tmpstr.toUInt(&ok, 16);
					// Check if memory address (low) is valid:
					if (ok)
					{
						// Get memory address (high) definition:
						tmpstr = mbdefline.section(';', 5, 5);
						if (!tmpstr.isEmpty())
							tmpMB.adr_high = tmpstr.toUInt(&ok, 16);
						// Check if memory address (high) is unused OR valid (only 16bit MBs):
						if ((tmpstr.isEmpty()) || (ok))	// if valid or no high byte memory address available
						{
							if (tmpstr.isEmpty())
								tmpMB.adr_high = 0;
							// Get title definition:
							tmpMB.title = mbdefline.section(';', 6, 6);
							// Check if title is available:
							if (!tmpMB.title.isEmpty())
							{
								// Get scaling formula definition:
								tmpMB.scaleformula = mbdefline.section(';', 8, 8);
								// Check if scaling formula is available:
								if (!tmpMB.scaleformula.isEmpty())
								{
									// Get precision and correct if necessary:
									tmpstr = mbdefline.section(';', 9, 9);
									tmpprecision = tmpstr.toInt(&ok, 10);
									if ((ok) && (tmpprecision < 4) && (tmpprecision > -4))
										tmpMB.precision = static_cast<char>(tmpprecision);
									else
										tmpMB.precision = 1;
									// Get unit:
									tmpMB.unit = mbdefline.section(';', 7, 7);
									// ***** MB IS SUPPORTED BY CU AND DEFINITION IS VALID *****
									// Put MB data on the list:
									supportedMBs->push_back(tmpMB);
								}
							}
						}
					}
				}
			}
		}
	}
}



void SSMprotocol::setupSupportedSWs(CUtype_dt CU, QString language, char flagbytes[96], unsigned char nrofflagbytes, std::vector<sw_intl_dt> *supportedSWs)
{
	QString swdefline;
	QString tmpstr;
	int tmpbytenr=0;
	int tmpbitnr=0;
	int tmpCUsupported=0;
	sw_intl_dt tmpSW;
	bool ok;
	int k;

	supportedSWs->clear();
	// Select definitions depending on language:
	QStringList swrawdata;
	if (language == "de")
	{
		SSMprotocol_def_de rawdefs_de;
		swrawdata = rawdefs_de.SWrawDefs();
	}
	else
	{
		SSMprotocol_def_en rawdefs_en;
		swrawdata = rawdefs_en.SWrawDefs();
	}
	// Assort list of supported switches:
	for (k=0; k<swrawdata.size(); k++)
	{
		// Get flagbyte address definition:
		swdefline = swrawdata.at(k);
		tmpstr = swdefline.section(';', 0, 0);
		tmpbytenr = tmpstr.toInt(&ok, 10);
		// Check if flagbyte address is defined for our CU:
		if (ok && (tmpbytenr <= nrofflagbytes))
		{
			// Get flagbit definition:
			tmpstr = swdefline.section(';', 1, 1);
			tmpbitnr = tmpstr.toInt();
			// Check if CU supports this switch (if flagbit is set):
			if ((tmpbitnr < 1) || (tmpbitnr > 8))
				ok = false;
			if ((ok) && (flagbytes[tmpbytenr-1] == (flagbytes[tmpbytenr-1] | static_cast<unsigned char>(pow(2, (tmpbitnr-1)) ))))
			{
				// Check if switch is intended for this CU type:
				tmpCUsupported = 0;
				if (CU == (CU | ECU))
				{
					tmpstr = swdefline.section(';', 2, 2);
					tmpCUsupported = tmpstr.toInt();
				}
				if ((tmpCUsupported != 1) && (CU == (CU | TCU)))	// if switch is intended for TCUs (and not for ECUs)
				{
					tmpstr = swdefline.section(';', 3, 3);
					tmpCUsupported = tmpstr.toInt();
				}
				if (tmpCUsupported == 1)
				{
					// Get memory address definition:
					tmpstr = swdefline.section(';', 4, 4);
					tmpSW.byteadr = tmpstr.toInt(&ok, 16);
					// Check if memory address is valid:
					if (ok)
					{
						// Get title definition:
						tmpSW.title = swdefline.section(';', 5, 5);
						// Check if title is available:
						if (!tmpSW.title.isEmpty())
						{
							// Get unit definition:
							tmpSW.unit = swdefline.section(';', 6, 6);
							if (!tmpSW.unit.isEmpty())
							{
								// ***** SWITCH IS SUPPORTED BY CU AND DEFINITION IS VALID *****
								// Put switch data on the list:
								tmpSW.bitadr = static_cast<unsigned char>(tmpbitnr);
								supportedSWs->push_back(tmpSW);
							}
						}
					}
				}
			}
		}
	}
}



void SSMprotocol::setupAdjustmentsData(CUtype_dt CU, QString language, char flagbytes[96], unsigned char nrofflagbytes, std::vector<adjustment_intl_dt> *adjustments)
{
	QString defline = "";
	QString tmphelpstr = "";
	unsigned int tmpflagbyte = 0;
	unsigned int tmpflagbit = 0;
	unsigned int tmpsidbval = 0;
	unsigned int tmpCU = 0;
	adjustment_intl_dt tmpadjustment;
	int k = 0;
	bool ok = false;
	bool supported = false;
	QStringList adjustmentsrawdata;

	adjustments->clear();
	if (language == "de")
	{
		SSMprotocol_def_de rawdefs_de;
		adjustmentsrawdata = rawdefs_de.AdjustmentRawDefs();
	}
	else
	{
		SSMprotocol_def_en rawdefs_en;
		adjustmentsrawdata = rawdefs_en.AdjustmentRawDefs();
	}
	for (k=0; k<adjustmentsrawdata.size(); k++)
	{
		defline = adjustmentsrawdata.at(k);
		tmphelpstr = defline.section(';', 0, 0);
		supported = false;
		if (tmphelpstr.count('-') == 1)
		{
			tmpflagbyte = tmphelpstr.section('-', 0, 0).toUInt(&ok, 10);
			if (ok && (tmpflagbyte > 0) && (tmpflagbyte < nrofflagbytes))
			{
				tmpflagbit = tmphelpstr.section('-', 1, 1).toUInt(&ok, 10);
				if (ok && (tmpflagbit > 0) && (tmpflagbit < 9))
				{
					if (flagbytes[tmpflagbyte-1] == (flagbytes[tmpflagbyte-1] | static_cast<unsigned char>(pow(2,(tmpflagbit-1)))))
						supported = true;
				}
			}
		}
		else if (tmphelpstr.size() == 6)
		{
			tmpsidbval = tmphelpstr.mid(0, 2).toUInt(&ok, 16);
			if (ok && (tmpsidbval == static_cast<unsigned char>(_SYS_ID[0])))
			{
				tmpsidbval = tmphelpstr.mid(2, 2).toUInt(&ok, 16);
				if (ok && (tmpsidbval == static_cast<unsigned char>(_SYS_ID[1])))
				{
					tmpsidbval = tmphelpstr.mid(4, 2).toUInt(&ok, 16);
					if (ok && (tmpsidbval == static_cast<unsigned char>(_SYS_ID[2])))
						supported = true;
				}
			}
		}
		if (supported)
		{
			tmpCU = defline.section(';', 1, 1).toUInt(&ok, 10);
			if (ok)
			{
				if ( ((CU == ECU) && (tmpCU == 0)) || ((CU == TCU) && (tmpCU == 1)) )
				{
					tmpadjustment.AddrLow = defline.section(';', 2, 2).toUInt(&ok, 16);
					if (ok && (tmpadjustment.AddrLow > 0))
					{
						tmpadjustment.AddrHigh = defline.section(';', 3, 3).toUInt(&ok, 16);
						if (!ok || (tmpadjustment.AddrHigh < 1))
							tmpadjustment.AddrHigh = 0;
						tmpadjustment.title = defline.section(';', 4, 4);
						if (tmpadjustment.title.length() > 0)
						{
							tmpadjustment.unit = defline.section(';', 5, 5);	// may be empty
							tmphelpstr = defline.section(';', 6, 6);
							tmpadjustment.rawMin = tmphelpstr.toUInt(&ok, 10);
							if (ok)
							{
								tmphelpstr = defline.section(';', 7, 7);
								tmpadjustment.rawMax = tmphelpstr.toUInt(&ok, 10);
								if (ok)
								{
									tmphelpstr = defline.section(';', 8, 8);
									tmpadjustment.rawDefault = tmphelpstr.toUInt(&ok, 10);
									if (ok)
									{
										tmpadjustment.formula = defline.section(';', 9, 9);
										if (tmpadjustment.formula.length() > 0)
										{
											tmphelpstr = defline.section(';', 10, 10);
											if (tmphelpstr.length() == 0)
												tmphelpstr = "0";
											tmpadjustment.precision = tmphelpstr.toInt(&ok, 10);
											if (ok && (tmpadjustment.precision<=10) && (tmpadjustment.precision>=-10))
												adjustments->push_back(tmpadjustment);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}



void SSMprotocol::setupActuatorTestData(QString language, actuator_dt *actuators, unsigned char *nrofActuators, unsigned int *allActByteAddr, unsigned char *nrofAllActByteAddr)
{
	QString tmpstr = "";
	unsigned int tmpflagbyte = 0;
	unsigned int tmpflagbit = 0;
	QString tmptitle = "";
	unsigned int tmpbyteadr = 0;
	unsigned char tmpbitadr = 0;
	bool fbvalid = true;
	bool ok = false;
	bool aol = false;
	int k = 0;
	int m = 0;
	QStringList actuatorsrawdata;

	*nrofActuators = 0;
	*allActByteAddr = 0;
	*nrofAllActByteAddr = 0;
	if (language == "de")
	{
		SSMprotocol_def_de rawdefs_de;
		actuatorsrawdata = rawdefs_de.ActuatorRawDefs();
	}
	else
	{
		SSMprotocol_def_en rawdefs_en;
		actuatorsrawdata = rawdefs_en.ActuatorRawDefs();
	}
	for (k=0; k<actuatorsrawdata.size(); k++)
	{
		fbvalid = true;
		tmpstr = actuatorsrawdata.at(k).section(';', 0, 0);
		if (tmpstr.size() > 0)
		{
			tmpflagbyte = tmpstr.toUInt(&ok, 10);
			if (!ok || (tmpflagbyte < 1) || (tmpflagbyte > _nrofflagbytes))
				fbvalid = false;
		}
		if (fbvalid)
		{
			tmpstr = actuatorsrawdata.at(k).section(';', 1, 1);
			if ((tmpstr.size() > 0) == (tmpflagbyte == 0))
			{
				fbvalid = false;
			}
			else if (tmpstr.size() > 0)
			{
				tmpflagbit = tmpstr.toUInt(&ok, 10);
				if (!ok || (tmpflagbit > 8))
					fbvalid = false;
				else
				{
					// Check if flagbyte is set:
					if (_flagbytes[tmpflagbyte-1] != (_flagbytes[tmpflagbyte-1] | static_cast<unsigned char>(pow(2, tmpflagbit-1))))
						fbvalid = false;
				}
			}
		}
		if (fbvalid)
		{
			tmpbyteadr = actuatorsrawdata.at(k).section(';', 2, 2).toUInt(&ok, 16);
			if (ok && (tmpbyteadr > 0))
			{
				tmpbitadr = actuatorsrawdata.at(k).section(';', 3, 3).toUInt(&ok, 10);
				if (ok && (tmpbitadr > 0) && (tmpbitadr < 9))
				{
					tmptitle = actuatorsrawdata.at(k).section(';', 4, 4);
					if (tmptitle.length() > 0)
					{
						actuators[*nrofActuators].title = tmptitle;
						actuators[*nrofActuators].byteadr = tmpbyteadr;
						actuators[*nrofActuators].bitadr = tmpbitadr;
						(*nrofActuators)++;
						// Check if byte address is already on the list:
						for (m=0; m<*nrofAllActByteAddr; m++)
						{
							if (allActByteAddr[m] == tmpbyteadr)
							{
								aol = true;
								break;
							}
						}
						// Put address on addresses list (if not duplicate):
						if (!aol)
						{
							allActByteAddr[*nrofAllActByteAddr] = tmpbyteadr;
							(*nrofAllActByteAddr)++;
						}
						else
							aol = false;
					}
				}
			}
		}
	}
}



bool SSMprotocol::getSupportedDCgroups(int *DCgroups)
{
	bool supported = false;
	if (_state == state_needSetup) return false;
	*DCgroups = 0;
	if (_nrofDTCsAddr > 0)
		(*DCgroups) += (temporaryDTCs_DCgroup + memorizedDTCs_DCgroup);
	if (!hasIntegratedCC(&supported))
		return false;
	if (supported)
	{
		(*DCgroups) += CClatestCCs_DCgroup;
		if (!hasIntegratedCCmemorizedCodes(&supported))
			return false;
		if (supported)
			(*DCgroups) += CCmemorizedCCs_DCgroup;
	}
	return true;
}



bool SSMprotocol::getLastDCgroupsSelection(int *DCgroups)
{
	if (_state == state_needSetup) return false;
	if (_selectedDCgroups != noDCs_DCgroup)
	{
		*DCgroups = _selectedDCgroups;
		return true;
	}
	return false;
}



bool SSMprotocol::getSupportedMBs(std::vector<mbsw_dt> *supportedMBs)
{
	if (_state == state_needSetup) return false;
	supportedMBs->clear();
	for (unsigned int k=0; k<_supportedMBs.size(); k++)
		supportedMBs->push_back( _supportedMBs.at(k) );
	return true;
}



bool SSMprotocol::getSupportedSWs(std::vector<mbsw_dt> *supportedSWs)
{
	if (_state == state_needSetup) return false;
	supportedSWs->clear();
	for (unsigned int k=0; k<_supportedSWs.size(); k++)
		supportedSWs->push_back( _supportedSWs.at(k) );
	return true;
}



bool SSMprotocol::getLastMBSWselection(std::vector<MBSWmetadata_dt> *MBSWmetaList)
{
	if (_state == state_needSetup) return false;
	if (!_MBSWmetaList.empty())
	{
		*MBSWmetaList = _MBSWmetaList;
		return true;
	}
	return false;
}



bool SSMprotocol::getSupportedAdjustments(std::vector<adjustment_dt> *supportedAdjustments)
{
	if (_state == state_needSetup) return false;
	supportedAdjustments->clear();
	for (unsigned int k=0; k<_adjustments.size(); k++)
		supportedAdjustments->push_back( _adjustments.at(k) );
	return true;
}



bool SSMprotocol::getSupportedActuatorTests(QStringList *actuatorTestTitles)
{
	unsigned char k = 0;
	bool ATsup = false;

	if (hasActuatorTests(&ATsup))
	{
		if (ATsup)
		{
			actuatorTestTitles->clear();
			for (k=0; k<_nrofActuators; k++)
				actuatorTestTitles->append(_actuators[k].title);
			return true;
		}
	}
	return false;
}



bool SSMprotocol::getLastActuatorTestSelection(unsigned char *actuatorTestIndex)
{
	bool ATsup = false;

	if (hasActuatorTests(&ATsup))
	{
		if (ATsup && (_selectedActuatorTestIndex != 255))
		{
			*actuatorTestIndex = _selectedActuatorTestIndex;
			return true;
		}
	}
	return false;
}




bool SSMprotocol::getAdjustmentValue(unsigned char index, unsigned int *rawValue)
{
	unsigned int dataaddr[2] = {0,0};
	unsigned int datalen = 0;
	char data[2] = {0,0};
	if (_state != state_normal) return false;
	// Validate adjustment value selection:
	if (_adjustments.empty() || (index >= _adjustments.size())) return false;
	// Convert memory address into two byte addresses:
	dataaddr[0] = _adjustments.at(index).AddrLow;
	datalen = 1;
	if (_adjustments.at(index).AddrHigh > 0)
	{
		dataaddr[1] = _adjustments.at(index).AddrLow;
		datalen++;
	}
	// Read data from control unit:
	if (!_SSMPcom->readMultipleDatabytes('\x0', dataaddr, datalen, data))
	{
		resetCUdata();
		return false;
	}
	// Calculate raw value:
	*rawValue = static_cast<unsigned char>(data[0]);
	if (_adjustments.at(index).AddrHigh > 0)
		*rawValue += 256*static_cast<unsigned char>(data[1]);
	return true;
}



bool SSMprotocol::getAllAdjustmentValues(unsigned int * rawValues)
{
	unsigned int dataaddr[2*SSMP_MAX_ADJUSTMENTS] = {0,};
	unsigned int datalen = 0;
	char data[2*SSMP_MAX_ADJUSTMENTS] = {0,};
	unsigned char k = 0;
	unsigned int addrindex = 0;
	if ((_state != state_normal) || _adjustments.empty()) return false;
	// Setup address list:
	for (k=0; k<_adjustments.size(); k++)
	{
		dataaddr[datalen] = _adjustments.at(k).AddrLow;
		datalen++;
		if (_adjustments.at(k).AddrHigh > 0)
		{
			dataaddr[datalen] = _adjustments.at(k).AddrHigh;
			datalen++;
		}
	}
	// Read data from control unit:
	if (!_SSMPcom->readMultipleDatabytes('\x0', dataaddr, datalen, data))
	{
		resetCUdata();
		return false;
	}
	// Calculate raw value:
	for (k=0; k<_adjustments.size(); k++)
	{
		rawValues[k] = static_cast<unsigned char>(data[addrindex]);
		addrindex++;
		if (_adjustments.at(k).AddrHigh > 0)
		{
			rawValues[k] += 256*static_cast<unsigned char>(data[addrindex]);
			addrindex++;
		}
	}
	return true;
}



bool SSMprotocol::setAdjustmentValue(unsigned char index, unsigned int rawValue)
{
	char lowdatabyte = 0;
	char highdatabyte = 0;
	if (_state != state_normal) return false;
	// Validate adjustment value selection:
	if (index >= _adjustments.size()) return false;
	if (((_adjustments.at(index).rawMin < _adjustments.at(index).rawMax) && (rawValue < _adjustments.at(index).rawMin)) || ((rawValue > _adjustments.at(index).rawMax)))
		return false;
	if ((_adjustments.at(index).rawMin > _adjustments.at(index).rawMax) && (rawValue < _adjustments.at(index).rawMin) && (rawValue > _adjustments.at(index).rawMax))
		return false;
	if ((_adjustments.at(index).AddrHigh > 0) && (rawValue > 65535))
	{
		return false;
	}
	else if (rawValue > 255)
	{
		return false;
	}
	// Convert raw value to 2 byte values:
	lowdatabyte = static_cast<char>(rawValue & 0xff);
	if (_adjustments.at(index).AddrHigh > 0)
		highdatabyte = static_cast<char>((rawValue & 0xffff) >> 8);
	// Write value to control unit:
	if (_adjustments.at(index).AddrHigh > 0)
	{
		if (!_SSMPcom->writeDatabyte(_adjustments.at(index).AddrHigh, highdatabyte))
		{
			resetCUdata();
			return false;
		}
	}
	if (!_SSMPcom->writeDatabyte(_adjustments.at(index).AddrLow, lowdatabyte))
	{
		resetCUdata();
		return false;
	}
	return true;
}




bool SSMprotocol::getVIN(QString *VIN)
{
	bool VINsup = false;
	char vin[18] = {0,};
	char vinaddrdata[4] = {0,};
	unsigned int dataaddr[3];
	dataaddr[0] = 0xDA;
	dataaddr[1] = 0xDB;
	dataaddr[2] = 0xDC;
	unsigned int vinstartaddr = 0;
	unsigned int vinaddr[17] = {0,};
	unsigned int k = 0;
	if (!hasVINsupport(&VINsup))
		return false;
	if (!VINsup) return false;
	VIN->clear();
	if (_SSMPcom->readMultipleDatabytes(0x0, dataaddr, 3, vinaddrdata))
	{
		vinstartaddr = (65536 * static_cast<unsigned char>(vinaddrdata[0]))
				+ (256 * static_cast<unsigned char>(vinaddrdata[1]))
				+ (static_cast<unsigned char>(vinaddrdata[2]));
		for (k=1; k<=17; k++)
			vinaddr[k-1] = vinstartaddr+k-1;
		if (_SSMPcom->readMultipleDatabytes(0x0, vinaddr, 17, vin))
		{
			if (validateVIN(vin))
			{
				vin[17]='\x0';
				*VIN = static_cast<QString>(vin);
			}
			return true;
		}
	}
	// Communication error:
	resetCUdata();
	return false;
}



bool SSMprotocol::isEngineRunning(bool *isrunning)
{
	unsigned int dataadr = 0x0e;
	char currentdatabyte = 0;
	if (_state != state_normal) return false;
	if (_flagbytes[0] != (_flagbytes[0] | 0x01)) return false;
	if (!_SSMPcom->readMultipleDatabytes(0x0, &dataadr, 1, &currentdatabyte))
	{
		resetCUdata();
		return false;
	}
	if (currentdatabyte > 3)
		*isrunning = true;
	else
		*isrunning = false;
	return true;
}



bool SSMprotocol::isInTestMode(bool *testmode)
{
	bool TMsup = false;
	unsigned int dataadr = 0x61;
	char currentdatabyte = 0;
	if (_state != state_normal) return false;
	if (!hasTestMode(&TMsup) || !TMsup) return false;
	if (!_SSMPcom->readMultipleDatabytes(0x0, &dataadr, 1, &currentdatabyte))
	{
		resetCUdata();
		return false;
	}
	if (currentdatabyte == (currentdatabyte | 0x20))
		*testmode = true;
	else
		*testmode = false;
	return true;
}



bool SSMprotocol::ClearMemory(CMlevel_dt level, bool *success)
{
	char val = 0;
	char bytewritten = 0;
	bool CM2sup = false;
	if (_state != state_normal) return false;
	if (level == CMlevel_1)
	{
		val = 0x40;
	}
	else if (level == CMlevel_2)
	{
		if (!hasClearMemory2(&CM2sup) || CM2sup==false) return false;
		val = 0x20;
	}
	else
	{
		return false;
	}
	*success = false;
	if (!_SSMPcom->writeDatabyte(0x000060, val, &bytewritten))
	{
		resetCUdata();
		return false;
	}
	if (bytewritten == val)
		*success = true;
	return true;
}



bool SSMprotocol::TestImmobilizerCommLine(immoTestResult_dt *result)
{
	bool ImmoSup = false;
	if (_state != state_normal) return false;
	if (!hasImmobilizer(&ImmoSup) || ImmoSup==false) return false;
	char readcheckvalue = 0;
	unsigned int readcheckadr = 0x8B;
	// Write test-pattern:
	_SSMPcom->writeDatabyte(0xE0, '\xAA', NULL);
	// Read result:
	if (_SSMPcom->readMultipleDatabytes('\x0', &readcheckadr, 1, &readcheckvalue))
	{
		if (readcheckvalue == '\x01')
		{
			*result = immoShortedToGround;
		}
		else if  (readcheckvalue == '\x02')
		{
			*result = immoShortedToBattery;
		}
		else
		{
			*result = immoNotShorted;
		}
		return true;
	}
	// Communication error:
	resetCUdata();
	return false;
}





bool SSMprotocol::startDCreading(int DCgroups, bool ignoreDCheckState)
{
	unsigned int DCqueryAddrList[(2*SSMP_MAX_DTCADDR)+(2*SSMP_MAX_CCCCADDR)] = {0};
	unsigned int DCqueryAddrListLen = 0;
	unsigned char k = 0;
	bool CCsup = false;
	bool CCmemSup = false;
	bool started;
	// Check if another communication operation is in progress:
	if (_state != state_normal) return false;
	// Try to determine the supported Cruise Control Cancel Code groups:
	if (!hasIntegratedCC(&CCsup) || !hasIntegratedCCmemorizedCodes(&CCmemSup)) return false;
	// Check argument:
	if (DCgroups < 1 || DCgroups > 15) return false;
	if (DCgroups > 3)
	{
		if (!CCsup)
			return false;
		if ( (DCgroups > 7) && (!CCmemSup) )
			return false;
	}
	// Setup diagnostic codes addresses list:
	if (DCgroups == (DCgroups | temporaryDTCs_DCgroup))	// current/temporary DTCs
	{
		if ((_CU == ECU) && !ignoreDCheckState)
		{
			DCqueryAddrList[DCqueryAddrListLen] = 0x000061;
			DCqueryAddrListLen++;
		}
		for (k=0; k<_nrofDTCsAddr; k++)
		{
			DCqueryAddrList[DCqueryAddrListLen] = _temporaryDTCsAddr[k];
			DCqueryAddrListLen++;
		}
	}
	if (DCgroups == (DCgroups | memorizedDTCs_DCgroup))	// historic/memorized DTCs
	{
		for (k=0; k<_nrofDTCsAddr; k++)
		{
			DCqueryAddrList[DCqueryAddrListLen] = _memorizedDTCsAddr[k];
			DCqueryAddrListLen++;
		}
	}
	if (CCsup)
	{
		if (DCgroups == (DCgroups | CClatestCCs_DCgroup))	// CC latest cancel codes
		{
			for (k=0; k<_nrofLatestCCCCsAddr; k++)
			{
				DCqueryAddrList[DCqueryAddrListLen] = _latestCCCCsAddr[k];
				DCqueryAddrListLen++;
			}
		}
		if ((DCgroups == (DCgroups | CCmemorizedCCs_DCgroup)) && CCmemSup)	// CC memorized cancel codes
		{
			for (k=0; k<_nrofMemorizedCCCCsAddr; k++)
			{
				DCqueryAddrList[DCqueryAddrListLen] = _memorizedCCCCsAddr[k];
				DCqueryAddrListLen++;
			}
		}
	}
	// Check if min. 1 Address to read:
	if ((DCqueryAddrListLen < 1) || ((DCqueryAddrList[0] == 0x000061) && (DCqueryAddrListLen < 2)))
		return false;
	// Start diagostic code reading:
	started = _SSMPcom->readMultipleDatabytes_permanent('\x0', DCqueryAddrList, DCqueryAddrListLen);
	if (started)
	{
		_state = state_DCreading;
		// Save diagnostic codes group selection (for data evaluation and restartDCreading()):
		_selectedDCgroups = DCgroups;
		_ignoreDCheckStateOnDCreading = ignoreDCheckState;
		// Connect signals and slots:
		connect( _SSMPcom, SIGNAL( recievedData(QByteArray, int) ),
			this, SLOT( processDCsRawdata(QByteArray, int) ), Qt::BlockingQueuedConnection );
		// Emit signal:
		emit startedDCreading();
	}
	else
		resetCUdata();
	return started;
}



bool SSMprotocol::restartDCreading()
{
	return startDCreading(_selectedDCgroups, _ignoreDCheckStateOnDCreading);
}



bool SSMprotocol::stopDCreading()
{
	if ((_state == state_needSetup) || (_state == state_normal)) return true;
	if (_state == state_DCreading)
	{
		if (_SSMPcom->stopCommunication())
		{
			disconnect( _SSMPcom, SIGNAL( recievedData(QByteArray, int) ),
				    this, SLOT( processDCsRawdata(QByteArray, int) ) );
			_state = state_normal;
			emit stoppedDCreading();
			return true;
		}
		// Communication error:
		resetCUdata();
	}
	return false;
}



void SSMprotocol::processDCsRawdata(QByteArray DCrawdata, int duration_ms)
{
	QStringList DCs;
	QStringList DCdescriptions;
	QStringList tmpDTCs;
	QStringList tmpDTCsDescriptions;
	unsigned int DCsAddrIndex = 0;
	unsigned int DCsAddrIndexOffset = 0;
	duration_ms = 0; // to avoid compiler error
	if (_selectedDCgroups == (_selectedDCgroups | temporaryDTCs_DCgroup))
	{
		if ((_CU == ECU) && !_ignoreDCheckStateOnDCreading)
			DCsAddrIndexOffset = 1;
		DCs.clear();
		DCdescriptions.clear();
		if ((_CU != ECU) || _ignoreDCheckStateOnDCreading || (DCrawdata.at(0) != (DCrawdata.at(0) | 0x80)))
		{
			// Evaluate current/latest data trouble codes:
			for (DCsAddrIndex=0; DCsAddrIndex<_nrofDTCsAddr; DCsAddrIndex++)
			{
				evaluateDCdataByte(_temporaryDTCsAddr[DCsAddrIndex], DCrawdata.at(DCsAddrIndexOffset+DCsAddrIndex), _DTC_rawDefs, &tmpDTCs, &tmpDTCsDescriptions);
				DCs += tmpDTCs;
				DCdescriptions += tmpDTCsDescriptions;
			}
		}
		else
		{
			DCs += "";
			DCdescriptions += tr("----- SYSTEM CHECK IS NOT YET COMPLETED ! -----");
		}
		DCsAddrIndexOffset += _nrofDTCsAddr;
		emit temporaryDTCs(DCs, DCdescriptions);
	}
	if (_selectedDCgroups == (_selectedDCgroups | memorizedDTCs_DCgroup))
	{
		DCs.clear();
		DCdescriptions.clear();
		// Evaluate historic/memorized data trouble codes:
		for (DCsAddrIndex=0; DCsAddrIndex<_nrofDTCsAddr; DCsAddrIndex++)
		{
			evaluateDCdataByte(_memorizedDTCsAddr[DCsAddrIndex], DCrawdata.at(DCsAddrIndexOffset+DCsAddrIndex), _DTC_rawDefs, &tmpDTCs, &tmpDTCsDescriptions);
			DCs += tmpDTCs;
			DCdescriptions += tmpDTCsDescriptions;
		}
		DCsAddrIndexOffset += _nrofDTCsAddr;
		emit memorizedDTCs(DCs, DCdescriptions);
	}
	if (_selectedDCgroups == (_selectedDCgroups | CClatestCCs_DCgroup))
	{
		DCs.clear();
		DCdescriptions.clear();
		// Evaluate latest CC cancel codes:
		for (DCsAddrIndex=0; DCsAddrIndex<_nrofLatestCCCCsAddr; DCsAddrIndex++)
		{
			evaluateDCdataByte(_latestCCCCsAddr[DCsAddrIndex], DCrawdata.at(DCsAddrIndexOffset+DCsAddrIndex), _CC_rawDefs, &tmpDTCs, &tmpDTCsDescriptions);
			DCs += tmpDTCs;
			DCdescriptions += tmpDTCsDescriptions;
		}
		DCsAddrIndexOffset += _nrofLatestCCCCsAddr;
		emit latestCCCCs(DCs, DCdescriptions);
	}
	if (_selectedDCgroups == (_selectedDCgroups | CCmemorizedCCs_DCgroup))
	{
		DCs.clear();
		DCdescriptions.clear();
		// Evaluate memorized CC cancel codes:
		for (DCsAddrIndex=0; DCsAddrIndex<_nrofMemorizedCCCCsAddr; DCsAddrIndex++)
		{
			evaluateDCdataByte(_memorizedCCCCsAddr[DCsAddrIndex], DCrawdata.at(DCsAddrIndexOffset+DCsAddrIndex), _CC_rawDefs, &tmpDTCs, &tmpDTCsDescriptions);
			DCs += tmpDTCs;
			DCdescriptions += tmpDTCsDescriptions;
		}
		emit memorizedCCCCs(DCs, DCdescriptions);
	}
}



void SSMprotocol::evaluateDCdataByte(unsigned int DCbyteadr, char DCrawdata, QStringList DC_rawDefs, QStringList *DC, QStringList *DCdescription)
{
	unsigned char setbits[8] = {0,};
	unsigned char setbitslen = 0;
	QStringList tmpdefparts;
	unsigned int tmpcurrentdtcadr = 0;
	unsigned int tmphistoricdtcadr = 0;
	unsigned int tmpbitadr = 0;
	int k = 0;
	unsigned char setbitsindex = 0;
	bool ok = false;

	DC->clear();
	DCdescription->clear();
	if (DCrawdata == 0) return;
	// Create list of set flagbits:
	unsigned char flagbit = 0;
	for (flagbit=1; flagbit<9; flagbit++)
	{
		if (DCrawdata == (DCrawdata | static_cast<char>(pow(2, (flagbit-1)))))
		{
			setbits[setbitslen] = flagbit;
			setbitslen++;
		}
	}
	// *** Search for matching DC definition ***:
	for (k=0; k<DC_rawDefs.size(); k++)      // work through all DC definition raw data
	{
		/* NOTE:	- unknown/reserved DCs have a definition with description "UNKNOWN ..."
				- DCs with missing definitions are ignored				*/
		tmpdefparts = DC_rawDefs.at(k).split(';');
		if (tmpdefparts.size() == 5)
		{
			tmpcurrentdtcadr = tmpdefparts.at(0).toUInt(&ok, 16);  // current/latest DCs memory address
			tmphistoricdtcadr = tmpdefparts.at(1).toUInt(&ok, 16); // historic/memorized DCs memory address
			if ((ok) && ((tmpcurrentdtcadr == DCbyteadr) || (tmphistoricdtcadr == DCbyteadr)))
			{
				tmpbitadr = tmpdefparts.at(2).toUInt(); // flagbit
				for (setbitsindex=0; setbitsindex<setbitslen; setbitsindex++)
				{
					// Check if definition belongs to current DC:
					if (tmpbitadr == setbits[setbitsindex])
					{
						DC->push_back(tmpdefparts.at(3));		// DC
						DCdescription->push_back(tmpdefparts.at(4));	// DC description
					}
				}
			}
		}
	}
}






bool SSMprotocol::startMBSWreading(std::vector<MBSWmetadata_dt> mbswmetaList)
{
	bool started = false;
	if (_state != state_normal) return false;
	// Setup list of MB/SW-addresses for SSMPcommunication:
	if (!setupMBSWQueryAddrList(mbswmetaList, _supportedMBs, _supportedSWs, _selMBsSWaAddr, &_selMBsSWsAddrLen))
		return false;
 	// Start MB/SW-reading:
	started = _SSMPcom->readMultipleDatabytes_permanent('\x0', _selMBsSWaAddr, _selMBsSWsAddrLen);
	if (started)
	{
		_state = state_MBSWreading;
		// Save MB/SW-selection (necessary for evaluation of raw data):
		_MBSWmetaList = mbswmetaList;
		// Connect signals/slots:
		connect( _SSMPcom, SIGNAL( recievedData(QByteArray, int) ),
			this, SLOT( processMBSWrawData(QByteArray, int) ) ); 
		// Emit signal:
		emit startedMBSWreading();
	}
	else
		resetCUdata();
	return started;
}



bool SSMprotocol::restartMBSWreading()
{
	return startMBSWreading(_MBSWmetaList);
}



bool SSMprotocol::stopMBSWreading()
{
	if ((_state == state_needSetup) || (_state == state_normal)) return true;
	if (_state == state_MBSWreading)
	{
		if (_SSMPcom->stopCommunication())
		{
			disconnect( _SSMPcom, SIGNAL( recievedData(QByteArray, int) ),
				    this, SLOT( processMBSWrawData(QByteArray, int) ) );
			_state = state_normal;
			emit stoppedMBSWreading();
			return true;
		}
		// Communication error:
		resetCUdata();
	}
	return false;
}



bool SSMprotocol::setupMBSWQueryAddrList(std::vector<MBSWmetadata_dt> MBSWmetaList, 
					 std::vector<mb_intl_dt> supportedMBs, std::vector<sw_intl_dt> supportedSWs, 
					 unsigned int *mbswadr, unsigned int *mbswadrlen)
{
	// ***** SETUP (BYTE-) ADDRESS LIST FOR QUERYS *****
	*mbswadrlen = 0;
	unsigned int k = 0, m = 0;
	bool newadr = true;
	if (MBSWmetaList.size() == 0) return false;
	for (k=0; k<MBSWmetaList.size(); k++)
	{
		newadr = true;
		// CHECK IF ADDRESS IS ALREADY ON THE QUERY-LIST:
		if (*mbswadrlen > 0)
		{
			// CHECK IF ADDRESS IS ALREADY ON THE LIST:
			for (m=0; (m<(*mbswadrlen)); m++)
			{
				if (MBSWmetaList.at(k).blockType == 0)
				{
					// CHECK IF CURRENT MB IS VALID/EXISTS:
					if (MBSWmetaList.at(k).nativeIndex > supportedMBs.size()) return false;
					// COMPARE ADDRESS:
					if (mbswadr[m] == (supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).adr_low))
					{
						newadr = false;
						break;
					}
				}
				else
				{
					// CHECK IF CURRENT SW IS VALID/EXISTS:
					if (MBSWmetaList.at(k).nativeIndex > supportedSWs.size()) return false;
					// COMPARE ADDRESS:
					if (mbswadr[m] == (supportedSWs.at( MBSWmetaList.at(k).nativeIndex ).byteadr))
					{
						newadr = false;
						break;
					}
				}
			}
		}
		// ADD ADDRESS TO QUERY-LIST IF IT IS NEW:
		if (newadr)
		{
			if (MBSWmetaList.at(k).blockType == 0)
			{
				// ADD ADDRESS(ES) OF CURRENT MB TO LIST:
				mbswadr[*mbswadrlen] = supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).adr_low;
				(*mbswadrlen)++;
				if (supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).adr_high > 0)
				{
					mbswadr[*mbswadrlen] = supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).adr_high;
					(*mbswadrlen)++;
				}
			}
			else
			{
				// ADD ADDRESS OF CURRENT SW TO LIST:
				mbswadr[*mbswadrlen] = supportedSWs.at( MBSWmetaList.at(k).nativeIndex ).byteadr;
				(*mbswadrlen)++;
			}
		}
	}
	return true;
}



void SSMprotocol::processMBSWrawData(QByteArray MBSWrawdata, int duration_ms)
{
	unsigned int rawValues[SSMP_MAX_MBSW] = {0};
	QStringList valueStrList;
	QStringList unitStrList;
	assignMBSWRawData( MBSWrawdata, _selMBsSWaAddr, _selMBsSWsAddrLen, _MBSWmetaList, _supportedMBs, _supportedSWs, rawValues );
	processMBSWRawValues( rawValues, _MBSWmetaList, _supportedMBs, _supportedSWs, &valueStrList, &unitStrList );
	emit newMBSWvalues( valueStrList, unitStrList, duration_ms );
}



void SSMprotocol::assignMBSWRawData(QByteArray rawdata, unsigned int mbswaddr[SSMP_MAX_MBSW], unsigned char mbswaddrlen,
				    std::vector<MBSWmetadata_dt> MBSWmetaList, std::vector<mb_intl_dt> supportedMBs, 
				    std::vector<sw_intl_dt> supportedSWs, unsigned int * mbswrawvalues)
{
	// ***** ASSIGN RAW DATA *****:
	unsigned int k = 0, m = 0;
	for (k=0; k<MBSWmetaList.size(); k++) mbswrawvalues[k]=0;
	for (m=0; m<mbswaddrlen; m++)	// ADDRESS LOOP
	{
		for (k=0; k<MBSWmetaList.size(); k++)	// MB/SW LOOP
		{
			if (MBSWmetaList.at(k).blockType == 0)
			{
				// COMPARE ADDRESSES:
				if (mbswaddr[m] == supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).adr_low)
				{
					// ADDRESS/RAW BYTE CORRESPONDS WITH LOW BYTE ADDRESS OF MB
					mbswrawvalues[k] += static_cast<unsigned char>(rawdata.at(m));
				}
				else if (mbswaddr[m] == supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).adr_high)
				{
					// ADDRESS/RAW BYTE CORRESPONDS WITH HIGH BYTE ADDRESS OF MB
					mbswrawvalues[k] += static_cast<unsigned char>(rawdata.at(m)) * 256;
				}
			}
			else
			{
				if (mbswaddr[m] == supportedSWs.at( MBSWmetaList.at(k).nativeIndex ).byteadr)
				{
					// ADDRESS/RAW BYTE CORRESPONS WITH BYTE ADDRESS OF SW
					if ( rawdata.at(m) == (rawdata.at(m) | static_cast<char>(pow(2, (supportedSWs.at( MBSWmetaList.at(k).nativeIndex ).bitadr -1) ) )) )	// IF ADDRESS BIT IS SET
						mbswrawvalues[k] = 1;
					else	// IF ADDRESS BIT IS NOT SET
						mbswrawvalues[k] = 0;
				}
			}
		}
	}
}



void SSMprotocol::processMBSWRawValues(unsigned int mbswrawvalues[SSMP_MAX_MBSW], std::vector<MBSWmetadata_dt> MBSWmetaList,
					std::vector<mb_intl_dt> supportedMBs, std::vector<sw_intl_dt> supportedSWs,
					QStringList *valueStrList, QStringList *unitStrList)
{
	double tmpscaledmbvalue = 0;
	unsigned int nrofDAs = 0;
	QString defstr;
	QString rvstr;
	bool da_success = false;
	unsigned int k = 0, m = 0;
	// RESET LISTS OF PROCESSED DATA:
	valueStrList->clear();
	unitStrList->clear();
	for (k=0; k<MBSWmetaList.size(); k++)
	{
		valueStrList->append("");
		unitStrList->append("");
	}
	// SCALE ALL MBs AND SWs:
	for (k=0; k<MBSWmetaList.size(); k++)	// MB/SW LOOP
	{
		if (MBSWmetaList.at(k).blockType == 0)
		{
			if (supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).scaleformula.contains('='))	// IF SCALE FORMULA CONTAINS SCALED VALUES (DIRECT ALLOCATION)
			{
				da_success = false;
				nrofDAs = 1 + (supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).scaleformula.count(','));
				for (m=0; m<nrofDAs; m++)
				{
					// GET NEXT ALLOCATION STRING:
					defstr = supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).scaleformula.section(',',m,m);
					// GET RAW VALUE OF ALLOCATION STRING:
					rvstr = defstr.section('=',0,0);
					// CHECK IF RAW VALUE MATCHES ALLOCATION:
					if ( mbswrawvalues[k] == (rvstr.toUInt()) )
					{
						// GET ALLOCATED VALUE:
						valueStrList->replace(k, defstr.section('=',1,1) );
						da_success = true;
						break;
					}
				}
				if (da_success)	// IF RAW VALUE HAS BEEN SUCCESFULLY ALLOCATED DIRECTLY
					// GET UNIT:
					unitStrList->replace(k, supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).unit);
				else	// IF DIRECT ALLOCATION OF RAW VALUE FAILED
				{
					// USE RAW VALUE:
					valueStrList->replace(k, QString::number(mbswrawvalues[k],10) );
					unitStrList->replace(k, "[RAW]");
				}
			}
			else if ( scaleMB( mbswrawvalues[k], supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).scaleformula, &tmpscaledmbvalue ) )	// TRY TO CALCULATE SCALED VALUE USING THE FORMULA
			{
				// GET PRECISION AND ROUND:
				switch (supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).precision)
				{
					case 0:
						valueStrList->replace(k, QString::number(tmpscaledmbvalue, 'f', 0));
						break;
					case 1:
						valueStrList->replace(k, QString::number(tmpscaledmbvalue, 'f', 1));
						break;
					case 2:
						valueStrList->replace(k, QString::number(tmpscaledmbvalue, 'f', 2));
						break;
					case 3:
						valueStrList->replace(k, QString::number(tmpscaledmbvalue, 'f', 3));
						break;
					default:
						valueStrList->replace(k, QString::number(tmpscaledmbvalue, 'f', 1));
				}
				// GET UNIT
				unitStrList->replace(k, supportedMBs.at( MBSWmetaList.at(k).nativeIndex ).unit);
			}
			else	// IF NO SCALE FORMULA AVAILABLE
			{
				// TAKE RAW VALUE:
				valueStrList->replace(k, QString::number(mbswrawvalues[k],10) );
				unitStrList->replace(k, "[unscaled]");
			}
		}
		else
		{
			// GET UNIT OF THE SWITCH:
			if (mbswrawvalues[k] == 0)
			{
				valueStrList->replace(k, supportedSWs.at( MBSWmetaList.at(k).nativeIndex ).unit.section('/',0,0));
			}
			else if (mbswrawvalues[k] == 1)
			{
				valueStrList->replace(k, supportedSWs.at( MBSWmetaList.at(k).nativeIndex ).unit.section('/',1,1));
			}
		}
	}
}



bool SSMprotocol::scaleMB(unsigned int rawvalue, QString scaleformula, double *scaledvalue)
{
	if (scaleformula.size() < 1) return false;
	*scaledvalue = rawvalue;
	if (scaleformula.startsWith("MSB", Qt::CaseInsensitive))
	{
		// GET BASIC SCALE FACTOR:
		double factor = scaleformula.section(':',1,1).toDouble();
		// CHECK IF FACTOR IS VALID:
		if (factor == 0) return false;
		// GET DEFINDED MSB (Most Significant Bit):
		int msb = scaleformula.section(':',0,0).mid(3).toInt();
		// CHECK IF MSB DEFINITION IS VALID:
		if ((msb != 8) && (msb != 16)) return false;
		// SCALE RAW VALUE:
		if (rawvalue > pow(2,msb-1) - 1)	// IF MSB IS SET
		{
			*scaledvalue = rawvalue - pow(2, msb);	// SUBSTRACT (HALF OF RANGE + 1)
		}
		*scaledvalue = (*scaledvalue) * factor;	// SCALE WITH BASIC FACTOR
	}
	else		// IF "NORMAL" FORMULA
	{
		unsigned char tmpvaluestrlen = 0;
		QString tmpvaluestr("");
		double tmpvalue = 0;
		char opchar = 0;
		unsigned char operatorindex[10] = {0,};
		unsigned char nrofoperators = 0;
		unsigned char calcstep = 0;
		unsigned char charindex = 0;
		bool ok = false;
		// CHECK FORMULA AND GET OPERATOR POSITIONS:
		for (charindex=0; charindex<scaleformula.size(); charindex++)
		{
			if ((scaleformula.at(charindex) == '+') || (scaleformula.at(charindex) == '-') || (scaleformula.at(charindex) == '*') || (scaleformula.at(charindex) == '/'))
			{
				ok = true;
				if (nrofoperators > 0)
				{
					// check for consectuive operators
					if (((charindex - operatorindex[nrofoperators-1]) < 2))
						ok = false;
					// the 2nd operator will be interpreted as prefix duricng conversion to double...
				}
				if (ok) // if operator should be put on the list
				{
					operatorindex[nrofoperators] = charindex;
					nrofoperators++;
				}
				/* NOTE: no further checks necessary, conversion to double will fail
				*       - if we have more then two consecutive operators
				*       - or if the second operator is not a + or - (prefix)
				*/
			}
			else if (charindex == 0) // IF FIRST CHARCTER IS NOT AN OPERATOR
			{
				return false;
			}
			else if (!scaleformula.at(charindex).isDigit())
			{
				return false;
			}
		}
		// CALCULATION:
		for (calcstep=0; calcstep<(nrofoperators); calcstep++)    // CALCULATION STEP LOOP
		{
			// EXTRACT NEXT OPERATOR:
			opchar = scaleformula.at( (operatorindex[calcstep]) ).toAscii();
			// GET LENGTH OF VALUE STRING:
			if (calcstep == (nrofoperators -1))	// IF LAST OPERATION/OPERATOR
				tmpvaluestrlen = scaleformula.size() - operatorindex[calcstep] -1;
			else
				tmpvaluestrlen = operatorindex[calcstep+1] - operatorindex[calcstep] -1;
			// CHECK VALUE STRING LENGTH:
			if (tmpvaluestrlen == 0) return false;
			// EXTRACT VALUE STRING AND cONVERT TO DOUBLE:
			tmpvaluestr = scaleformula.mid( operatorindex[calcstep]+1, tmpvaluestrlen );
			tmpvalue = tmpvaluestr.toDouble( &ok );
			if (!ok) return false;
			if (tmpvalue != 0)
			{
				// DO CALCUALTION STEP:
				switch (opchar)
				{
					case '+':
						*scaledvalue += tmpvalue;
						break;
					case '-':
						*scaledvalue -= tmpvalue;
						break;
					case '*':
						*scaledvalue *= tmpvalue;
						break;
					case '/':
						*scaledvalue /= tmpvalue;
						break;
					default:	// ERROR
						*scaledvalue = 0;
						return false;
				}
			}
		}
	}
	return true;
}





bool SSMprotocol::startActuatorTest(unsigned char actuatorTestIndex)
{
	bool ATsup = false;
	bool ATstarted = false;
	bool ok = false;
	bool testmode = false;
	bool running = false;
	unsigned char k = 0;
	// Check if another communication operation is in progress:
	if (_state != state_normal) return false;
	// Validate selected test:
	ok = hasActuatorTests(&ATsup);
	if (!ok || (ATsup == false) || (actuatorTestIndex >= _nrofActuators))
		return false;
	// Check if control unit is in test mode:
	ok = isInTestMode(&testmode);
	if (!ok || !testmode)
		return false;
	// Check that engine is not running:
	ok = isEngineRunning(&running);
	if (!ok || running)
		return false;
	// Change state:
	_state = state_ActTesting;
	// Prepare test addresses:
	unsigned int dataaddr = _actuators[actuatorTestIndex].byteadr;
	char databyte = static_cast<char>(pow(2, _actuators[actuatorTestIndex].bitadr - 1));
	// Stop all actuator tests:
	for (k=0; k<_nrofAllActByteAddr; k++)
	{
		if (!_SSMPcom->writeDatabyte(_allActByteAddr[k], 0x00))
		{
			_state = state_normal; // this avoids that resetCUdata() will try to stop all actuators again
			resetCUdata();
			return false;
		}
	}
	// Start Actuator Test:
	ATstarted = _SSMPcom->writeDatabyte_permanent(dataaddr, databyte, 100);
	if (ATstarted)
	{
		_selectedActuatorTestIndex = actuatorTestIndex;
		emit startedActuatorTest();
	}
	else
	{
		_state = state_normal; // this avoids that resetCUdata() will try to stop all actuators again
		resetCUdata();
	}
	return ATstarted;
}



bool SSMprotocol::restartActuatorTest()
{
	return startActuatorTest(_selectedActuatorTestIndex);
}



bool SSMprotocol::stopActuatorTesting()
{
	unsigned char k = 0;
	if ((_state == state_needSetup) || (_state == state_normal)) return true;
	if (_state == state_ActTesting)
	{
		if (_SSMPcom->stopCommunication())
		{
			// Stop all actuator tests:
			for (k=0; k<_nrofAllActByteAddr; k++)
			{
				if (!_SSMPcom->writeDatabyte(_allActByteAddr[k], 0x00))
				{
					resetCUdata();
					return false;
				}
			}
			_state = state_normal;
			emit stoppedActuatorTest();
			return true;
		}
		// Communication error:
		resetCUdata();
	}
	return false;
}



bool SSMprotocol::stopAllActuators()
{
	// NOTE: This function can be called even if no actuator test has been started with SSMprotocol
	// => When switching the cars ignition on (with engine off) while test mode connector is connected,
	//    some actuator tests are started automaticly
	unsigned char k = 0;
	bool ok = false;
	bool ATsup = false;
	bool testmode = false;
	bool enginerunning = false;
	if (_state != state_normal) return false;
	// Check if actuator tests are supported:
	ok = hasActuatorTests(&ATsup);
	if (!ok || !ATsup)
		return false;
	// Check if control unit is in test mode:
	ok = isInTestMode(&testmode);
	if (!ok || !testmode)
		return false;
	// Check that engine is not running:
	ok = isEngineRunning(&enginerunning);
	if (!ok || enginerunning)
		return false;
	// Stop all actuator tests:
	for (k=0; k<_nrofAllActByteAddr; k++)
	{
		if (!_SSMPcom->writeDatabyte(_allActByteAddr[k], 0x00))
		{
			resetCUdata();
			return false;
		}
	}
	return true;
}



bool SSMprotocol::stopAllPermanentOperations()
{
	bool result = false;
	if ((_state == state_needSetup) || (_state == state_normal))
	{
		result = true;
	}
	else if (_state == state_DCreading)
	{
		result = stopDCreading();
	}
	else if (_state == state_MBSWreading)
	{
		result = stopMBSWreading();
	}
	else if (_state == state_ActTesting)
	{
		result = stopActuatorTesting();
	}
	return result;
}



bool SSMprotocol::waitForIgnitionOff()
{
	if (_state != state_normal)
		return false;
	unsigned int dataadr = 0x62;
	_state = state_waitingForIgnOff;
	_SSMPcom->setRetriesOnError(1);
	if (_flagbytes[12] == (_flagbytes[12] | 0x08))	// MB "ignition switch"
	{
		bool ignstate = true;
		char data = 0x00;
		do
		{
			if(!_SSMPcom->readMultipleDatabytes('\x0', &dataadr, 1, &data))
				ignstate = false;
			else
				ignstate = (data == (data | 0x08));
		} while (ignstate);
	}
	else
	{
		QEventLoop el;
		disconnect( _SSMPcom, SIGNAL( commError() ), this, SIGNAL( commError() ) );
		disconnect( _SSMPcom, SIGNAL( commError() ), this, SLOT( resetCUdata() ) );
		if(!_SSMPcom->readMultipleDatabytes_permanent('\x0', &dataadr, 1))
		{
			resetCUdata();
			return false;
		}
		connect(_SSMPcom, SIGNAL( commError() ), &el, SLOT( quit() ));
		el.exec();
		disconnect(_SSMPcom, SIGNAL( commError() ), &el, SLOT( quit() ));
	}
	_SSMPcom->setRetriesOnError(2);
	resetCUdata();
	return true;
/* NOTE: temporary solution, will become obsolete with new SSMPcommunication */
}



bool SSMprotocol::validateVIN(char VIN[17])
{
	unsigned char k = 0;
	for (k=0; k<17; k++)
	{
		if ((VIN[k] < '\x30') || ((VIN[k] > '\x39') && (VIN[k] < '\x41')) || (VIN[k] > '\x5A'))
			return false;
		if ((k==5) || (k==8) || (k>10))
		{
			if (VIN[k] > '\x39')
				return false;
		}	
	}
	return true;
}



void SSMprotocol::StrToHexstr(char *inputstr, unsigned int nrbytes, QString *hexstr)
{
	unsigned short int charval = 0;
	unsigned char hexsigns[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	unsigned int bc = 1;
	hexstr->clear();
	for (bc=0; bc<nrbytes; bc++)
	{
		charval = static_cast<unsigned char>(inputstr[bc]);
		hexstr->append(hexsigns[charval/16]);
		hexstr->append(hexsigns[charval % 16]);
		if (bc != nrbytes - 1)
			hexstr->append(' ');
	}
	hexstr->append('\0');
}

