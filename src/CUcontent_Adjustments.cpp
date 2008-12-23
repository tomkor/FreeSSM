/*
 * CUcontent_Adjustments.cpp - Widget for Control Unit Adjustments
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

#include "CUcontent_Adjustments.h"




ModQDoubleSpinBox::ModQDoubleSpinBox(QWidget * parent) : QDoubleSpinBox(parent)
{
	_DVMbaseValue = 0;
	_discreteValuesMode = false;
	_precCorrVal = 0;
	recalcPrecCorrValue();
}


void ModQDoubleSpinBox::stepBy(int steps)
{
	double cpValue = 0, crValue = 0, newValue = 0; double delta = 0;
	if (_discreteValuesMode)
	{
		// Get current round value:
		crValue = pow(10,-decimals()) * round( value()*pow(10,decimals()) );
		if (singleStep() < pow(10, -decimals()))
		{
			// Calculate new value:
			newValue = crValue + (steps * pow(10, -decimals()));
			newValue = roundToNextDiscreteValue( newValue );
		}
		else
		{
			// Get current precise value:
			cpValue = roundToNextDiscreteValue( value() );
			// Correct nr of steps (if necessary):
			if (steps > 0)
				delta = cpValue - crValue;
			else // < 0 (= 0 ???)
				delta = crValue - cpValue;
			if (delta >= (pow(10, -decimals()) - _precCorrVal))
				steps -= steps/abs(steps);
			// Calculate new value:
			newValue = cpValue + (steps * singleStep());
		}
		// Set new value (automatic round to decimals):
		QDoubleSpinBox::setValue( newValue );
	}
	else
		QDoubleSpinBox::stepBy(steps);
}


void ModQDoubleSpinBox::setDiscreteValuesModeBaseValue(double baseVal)
{
	double crValue = 0;
	_DVMbaseValue = baseVal;
	recalcPrecCorrValue();
	if (_discreteValuesMode)
	{
		crValue = pow(10,-decimals()) * round( value()*pow(10,decimals()) );
		setValue( roundToNextDiscreteValue( crValue ) );
	}
}


void ModQDoubleSpinBox::setDiscreteValuesModeEnabled(bool enable)
{
	double crValue = 0;
	_discreteValuesMode = enable;
	if (_discreteValuesMode)
	{
		crValue = pow(10,-decimals()) * round( value()*pow(10,decimals()) );
		setValue( roundToNextDiscreteValue( crValue ) );
		connect(this, SIGNAL( editingFinished() ), this, SLOT( roundEditedValue() ) );
	}
	else
		disconnect(this, SIGNAL( editingFinished() ), this, SLOT( roundEditedValue() ) );
}


void ModQDoubleSpinBox::roundEditedValue()
{
	if (_discreteValuesMode) // to be sure...
		QDoubleSpinBox::setValue( roundToNextDiscreteValue( value() ) );
	/* NOTE: we don't have to round value() here, because it is already round to decimals() */
}


double ModQDoubleSpinBox::roundToNextDiscreteValue(double inValue)
{
	double d_new = 0;
	double kcorr = 0;
	if (singleStep() != 0)
	{
		if ((inValue >= 0))
			kcorr = _precCorrVal;
		if ((inValue < 0))
			kcorr = - _precCorrVal;
		d_new = singleStep() * round(((inValue - _DVMbaseValue + kcorr) / singleStep())) + _DVMbaseValue;
	}
	else
		d_new = inValue;
	return d_new;
}


void ModQDoubleSpinBox::recalcPrecCorrValue()
{
	int base_decimals = static_cast<int>(-floor(log10(_DVMbaseValue)));
	int singleStep_decimals = static_cast<int>(-floor(log10(singleStep())));
	int corrVal_decimals = 1 + std::max(decimals(), std::max(base_decimals, singleStep_decimals));
	_precCorrVal = pow(10,-corrVal_decimals);
}


void ModQDoubleSpinBox::setDecimals(int prec)
{
	double crValue = 0;
	QDoubleSpinBox::setDecimals(prec);
	recalcPrecCorrValue();
	if (_discreteValuesMode)
	{
		crValue = pow(10,-decimals()) * round( value()*pow(10,decimals()) );
		setValue( roundToNextDiscreteValue( crValue ) );
	}
}


void ModQDoubleSpinBox::setSingleStep(double val)
{
	double crValue = 0;
	QDoubleSpinBox::setSingleStep(val);
	recalcPrecCorrValue();
	if (_discreteValuesMode)
	{
		crValue = pow(10,-decimals()) * round( value()*pow(10,decimals()) );
		setValue( roundToNextDiscreteValue( crValue ) );
	}
}


void ModQDoubleSpinBox::setValue( double val )
{
	if (_discreteValuesMode)
		val = roundToNextDiscreteValue( val );
	QDoubleSpinBox::setValue(val);
}







QIdPushButton::QIdPushButton(const QString text, unsigned int indentifier, QWidget *parent) : QPushButton(text, parent)
{
	_indentifier = indentifier;
	connect (this, SIGNAL( pressed() ), this, SLOT( emitPressed() ));
};


void QIdPushButton::emitPressed()
{
	emit pressed(_indentifier);
}







CUcontent_Adjustments::CUcontent_Adjustments(QWidget *parent, SSMprotocol *SSMPdev) : QWidget(parent)
{
	QHeaderView *headerview;
	_SSMPdev = SSMPdev;
	_maxrowsvisible = 0; // We don't need to calculate a value here, because we always get a resizeEvent before setting the table content
	_supportedAdjustments.clear();
	_newValueSelWidgetType.clear();
	// Setup GUI:
	setupUi(this);
	setupUiFonts();
	// Set column widths:
	adjustments_tableWidget->setColumnWidth (1, 88);
	adjustments_tableWidget->setColumnWidth (2, 105);
	adjustments_tableWidget->setColumnWidth (3, 70);
	headerview = adjustments_tableWidget->horizontalHeader();
	headerview->setResizeMode(0,QHeaderView::Stretch);
	headerview->setResizeMode(1,QHeaderView::Fixed);
	headerview->setResizeMode(2,QHeaderView::Fixed);
	headerview->setResizeMode(3,QHeaderView::Fixed);
	// Install event-filter for adjustments-table:
	adjustments_tableWidget->viewport()->installEventFilter(this);
	// Disable GUI-elements:
	title_label->setEnabled(false);
	adjustments_tableWidget->setEnabled(false);
	nonPermanentInfo_label->setEnabled(false);
}



bool CUcontent_Adjustments::setup()
{
	unsigned int rawValues[SSMP_MAX_ADJUSTMENTS] = {0,};
	unsigned char k = 0;
	bool ok = false;
	bool calcerror = false;

	// Reset data:
	_supportedAdjustments.clear();
	_newValueSelWidgetType.clear();
	// Get supported adjustments:
	ok = _SSMPdev->getSupportedAdjustments(&_supportedAdjustments);
	if (ok && !_supportedAdjustments.empty())
	{
		// Determine the needed selection widget type for new values:
		setupNewValueSelWidgetTypes();
		// Setup adjustments table (without current values):
		setupAdjustmentsTable();
		// Query current adjustment values from CU:
		ok = _SSMPdev->getAllAdjustmentValues(rawValues);
		if (ok)
		{
			// Scale raw values:
			QString scaledValueString = "";
			for (k=0; k<_supportedAdjustments.size(); k++)
			{
				if (raw2scaled(rawValues[k], _supportedAdjustments.at(k).formula, _supportedAdjustments.at(k).precision, &scaledValueString))
					displayCurrentValue(k, scaledValueString, _supportedAdjustments.at(k).unit);
				else
				{
					calcerror = true;
					displayCurrentValue(k, QString::number(rawValues[k]), tr("[RAW]"));
				}
			}
		}
	}
	// Enable/Disable GUI-elements:
	title_label->setEnabled( ok );
	adjustments_tableWidget->setEnabled( ok );
	nonPermanentInfo_label->setEnabled( ok );
	// Check for calculation error(s):
	if (calcerror)
		calculationError(tr("One or more current values couldn't be scaled."));
	// Return result:
	return ok;
}



void CUcontent_Adjustments::setupNewValueSelWidgetTypes()
{
	bool combobox = false;
	QString formulaStr = "";
	QString defstr = "";
	QString svstr = "";
	bool ok = false;
	unsigned char k = 0;
	int m = 0;
	_newValueSelWidgetType.clear();
	for (k=0; k<_supportedAdjustments.size(); k++)
	{
		combobox = false;
		formulaStr = _supportedAdjustments.at(k).formula;
		if (formulaStr.contains('='))
		{
			for (m=0; m<=formulaStr.count(','); m++)
			{
				// Get next allocation string:
				defstr = formulaStr.section(',', m, m);
				// Get scaled part of the allocation string:
				svstr = defstr.section('=', 1, 1);
				// Try to convert scaled part to double:
				svstr.toDouble(&ok);
				if (!ok)
				{
					combobox = true;
					break;
				}
			}
		}
		_newValueSelWidgetType.push_back( combobox );
	}
}



void CUcontent_Adjustments::getSelectableScaledValueStrings(QString formulaStr, QStringList *selectableScaledValueStr)
{
	int k = 0;
	QString defstr = "";
	QString svstr = "";
	selectableScaledValueStr->clear();
	if (formulaStr.contains('='))
	{
		for (k=0; k<=formulaStr.count(','); k++)
		{
			// Get next allocation string:
			defstr = formulaStr.section(',', k, k);
			// Get scaled part of the allocation string:
			svstr = defstr.section('=', 1, 1);
			// Add value to the list of selectable scaled values:
			selectableScaledValueStr->append(svstr);
		}
	}
}



void CUcontent_Adjustments::setupAdjustmentsTable()
{
	bool enable = false;
	bool ok = false;
	unsigned char k = 0;
	QTableWidgetItem *tableItem = NULL;
	ModQDoubleSpinBox *spinBox = NULL;
	QComboBox *comboBox = NULL;
	QIdPushButton *saveButton = NULL;
	QPushButton *resetButton = NULL;
	QPixmap leftIcon, rightIcon;
	QPixmap mergedIcon(54, 22);
	QStringList selectableScaledValueStr;
	QString helpScaledValueStr = 0;
	double minScaledValue = 0;
	double maxScaledValue = 0;
	double defaultScaledValue = 0;
	double minSingleStepFromPrecision = 0;
	double minSingleStepFromRaw = 0;
	bool calcerror = false;

	// Create "Save"-icon:
	leftIcon.load( QString::fromUtf8(":/icons/oxygen/22x22/go-next.png") );
	rightIcon.load( QString::fromUtf8(":/icons/oxygen/22x22/drive-harddisk.png") );
	mergedIcon.fill(Qt::transparent);
	QPainter painter(&mergedIcon);
	painter.drawTiledPixmap ( 0, 0, 22, 22, leftIcon);
	painter.drawTiledPixmap ( 32, 0, 22, 22, rightIcon);
	QIcon saveButton_icon( mergedIcon );
	// Create "Reset"-icon:
	leftIcon.load( QString::fromUtf8(":/icons/oxygen/22x22/go-first.png") );
	mergedIcon.fill(Qt::transparent);
	painter.drawTiledPixmap ( 0, 0, 22, 22, leftIcon);
	painter.drawTiledPixmap ( 32, 0, 22, 22, rightIcon);
	QIcon resetButton_icon( mergedIcon );
	// Clear Table:
	adjustments_tableWidget->clearContents();
	// Enable/Disable GUI-elements:
	title_label->setEnabled( enable );
	adjustments_tableWidget->setEnabled( enable );
	nonPermanentInfo_label->setEnabled( enable );
	// Increase nr. of table rows if necessary:
	if (_maxrowsvisible < _supportedAdjustments.size())
		adjustments_tableWidget->setRowCount( _supportedAdjustments.size() );
	// Fill Table:
	for (k=0; k<_supportedAdjustments.size(); k++)
	{
		// Title:
		tableItem = new QTableWidgetItem( _supportedAdjustments.at(k).title );
		adjustments_tableWidget->setItem(k, 0, tableItem);
		// Current Value:
		tableItem = new QTableWidgetItem( "???" );
		tableItem->setTextAlignment(Qt::AlignCenter);
		adjustments_tableWidget->setItem(k, 1, tableItem);
		// New Value:
		if (_newValueSelWidgetType.at(k))
		{
			// Get selectable scaled values:
			selectableScaledValueStr.clear();
			getSelectableScaledValueStrings(_supportedAdjustments.at(k).formula, &selectableScaledValueStr);
			// Setup and insert selection-Combobox:
			comboBox = new QComboBox();
			comboBox->addItems( selectableScaledValueStr );
			adjustments_tableWidget->setCellWidget ( k, 2, comboBox );
		}
		else
		{
			// Calculate and set min/max:
			ok = raw2scaled(_supportedAdjustments.at(k).rawMin, _supportedAdjustments.at(k).formula, _supportedAdjustments.at(k).precision, &helpScaledValueStr);
			if (ok)
				minScaledValue = helpScaledValueStr.toDouble(&ok);
			if (ok)
				ok = raw2scaled(_supportedAdjustments.at(k).rawMax, _supportedAdjustments.at(k).formula, _supportedAdjustments.at(k).precision, &helpScaledValueStr);
			if (ok)
				maxScaledValue = helpScaledValueStr.toDouble(&ok);
			if (ok)
				ok = raw2scaled(_supportedAdjustments.at(k).rawDefault, _supportedAdjustments.at(k).formula, _supportedAdjustments.at(k).precision, &helpScaledValueStr);
			if (ok)
				defaultScaledValue = helpScaledValueStr.toDouble(&ok);
			if (!ok)
			{
				calcerror = true;
				continue; // continue with next adjustment value
			}
			spinBox = new ModQDoubleSpinBox();
			// put spinbox into the table:
			adjustments_tableWidget->setCellWidget ( k, 2, spinBox );
			/* NOTE: we do this here, because some spinbox functions don't work as expected (Qt-bugs ?) if spinBox is not visible yet */
			// Set adjustable range:
			if (minScaledValue > maxScaledValue)
				spinBox->setRange(maxScaledValue, minScaledValue);
			else
				spinBox->setRange(minScaledValue, maxScaledValue);
			// Calculate and set step size:
			minSingleStepFromPrecision = pow(10, (-1*_supportedAdjustments.at(k).precision));
			minSingleStepFromRaw = (maxScaledValue - minScaledValue) / (_supportedAdjustments.at(k).rawMax - _supportedAdjustments.at(k).rawMin);
			/* NOTE: this only works for constant step size ! */
			if (minSingleStepFromRaw > minSingleStepFromPrecision)
				spinBox->setSingleStep( minSingleStepFromRaw );
			else
				spinBox->setSingleStep( minSingleStepFromPrecision );
			// Set base value for "discrete values mode":
			spinBox->setDiscreteValuesModeBaseValue(defaultScaledValue);
			// Enable "discrete values mode":
			spinBox->setDiscreteValuesModeEnabled(true);
			// Set decimals:
			spinBox->setDecimals(_supportedAdjustments.at(k).precision);
			// Set suffix (unit):
			spinBox->setSuffix(" " + _supportedAdjustments.at(k).unit);
			// Set alignement:
			spinBox->setAlignment(Qt::AlignCenter);
		}
		// "Save"-button:
		saveButton = new QIdPushButton("", k, adjustments_tableWidget);
		saveButton->setIcon(saveButton_icon);
		saveButton->setIconSize( QSize(54,22) );
		connect (saveButton, SIGNAL( pressed(unsigned int) ), this, SLOT( saveAdjustmentValue(unsigned int) ));
		adjustments_tableWidget->setCellWidget ( k, 3, saveButton );	
	}
	// Setup "Reset all"-elements:
	if (_supportedAdjustments.size() > 0)
	{
		// Title:
		tableItem = new QTableWidgetItem( tr("Reset all: ") );
		tableItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
		adjustments_tableWidget->setItem(_supportedAdjustments.size()+1, 2, tableItem);
		// "Reset all"-button:
		resetButton = new QPushButton(adjustments_tableWidget);
		resetButton->setIcon( resetButton_icon );
		resetButton->setIconSize( QSize(54, 22) );
		connect (resetButton, SIGNAL( pressed() ), this, SLOT( resetAllAdjustmentValues() ));
		adjustments_tableWidget->setCellWidget ( _supportedAdjustments.size()+1, 3, resetButton );	
	}
	// Check for calculation error(s):
	if (calcerror)
		calculationError(tr("One or more values will not be adjustable to prevent\nwrong data being written to the Control Unit."));
}



void CUcontent_Adjustments::displayCurrentValue(unsigned char adjustment_index, QString currentValueStr, QString unit)
{
	QTableWidgetItem *tableItem;
	QString outputStr = "";
	QDoubleSpinBox *spinbox = NULL;
	QComboBox *combobox = NULL;
	QWidget *cellWidget = NULL;
	double sbvalue = 0;
	bool ok = false;

	if (currentValueStr.isEmpty())
		outputStr = "???";
	else
		outputStr = currentValueStr;
	// Append unit (if available):
	if ( unit != "" )
		outputStr += " " + unit;
	// Display value+unit:
	tableItem = new QTableWidgetItem( outputStr );
	tableItem->setTextAlignment(Qt::AlignCenter);
	adjustments_tableWidget->setItem(adjustment_index, 1, tableItem);
	// Set value of the selection-elements to the current value:
	if (!currentValueStr.isEmpty())
	{
		cellWidget = adjustments_tableWidget->cellWidget (adjustment_index, 2);
		if (_newValueSelWidgetType.at(adjustment_index))	// Combobox
		{
			combobox = dynamic_cast<QComboBox*>(cellWidget);
			int index = combobox->findText(currentValueStr);
			if (index > -1)
				combobox->setCurrentIndex(index);
		}
		else	// Spinbox
		{
			spinbox = dynamic_cast<ModQDoubleSpinBox*>(cellWidget);
			sbvalue = currentValueStr.toDouble(&ok);
			if (ok)
				spinbox->setValue( sbvalue );
		}
	}
}



void CUcontent_Adjustments::saveAdjustmentValue(unsigned int index)
{
	QWidget *cellWidget = NULL;
	QDoubleSpinBox *spinbox = NULL;
	QComboBox *combobox = NULL;
	double newvalue_scaledDouble = 0;
	QString newvalue_scaledStr = "";
	unsigned int newvalue_raw = 0;
	unsigned int controlValue_raw = 0;
	bool ok = false;

	// Show wait-message:
	FSSM_WaitMsgBox waitmsgbox(this, tr("Saving adjustment value to Electronic Control Unit... Please wait !      "));
	waitmsgbox.show();
	// Get selected Value from table:
	cellWidget = adjustments_tableWidget->cellWidget (index, 2);
	if (_newValueSelWidgetType.at(index))	// Combobox
	{
		combobox = dynamic_cast<QComboBox*>(cellWidget);
		newvalue_scaledStr = combobox->currentText();
		// Convert scaled value string to raw value:
		if (!scaled2rawByDirectAssociation(newvalue_scaledStr, _supportedAdjustments.at(index).formula, &newvalue_raw))
		{
			calculationError(tr("The new adjustment value couldn't be scaled."));
			return;
		}
	}
	else	// Spinbox
	{
		spinbox = dynamic_cast<ModQDoubleSpinBox*>(cellWidget);
		newvalue_scaledDouble = spinbox->value();
		newvalue_scaledStr = QString::number(newvalue_scaledDouble, 'f', _supportedAdjustments.at(index).precision);
		// Convert scaled value to raw value
		if (!scaled2raw(newvalue_scaledStr, _supportedAdjustments.at(index).formula, &newvalue_raw))
		{
			calculationError(tr("The new adjustment value couldn't be scaled."));
			return;
		}
	}
	// Save new ajustment value to control unit:
	ok = _SSMPdev->setAdjustmentValue(index, newvalue_raw);
	// To be sure: read and verify value again
	if (ok)
		ok = _SSMPdev->getAdjustmentValue(index, &controlValue_raw);
	if (!ok)
	{
		communicationError(tr("No or invalid answer from Control Unit."));
		return;
	}
	// Scale and display the current value:
	ok = raw2scaled(controlValue_raw, _supportedAdjustments.at(index).formula, _supportedAdjustments.at(index).precision, &newvalue_scaledStr);
	if (ok)
		displayCurrentValue(index, newvalue_scaledStr, _supportedAdjustments.at(index).unit);
	else
		displayCurrentValue(index, QString::number(controlValue_raw, 10), tr("[RAW]"));
	// Close wait-messagebox
	waitmsgbox.hide();
	// Check if the CU accepted the new value:
	if (controlValue_raw != newvalue_raw)
		errorMsg(tr("Error"), tr("Error:\nThe Control Unit didn't accept the new value !"));
	// Check for calculation error:
	if (!ok)
		calculationError(tr("The current value couldn't be scaled."));
}



void CUcontent_Adjustments::resetAllAdjustmentValues()
{
	int uc = 0;
	unsigned char k = 0;
	bool calcerror = false;

	// Show "Confirm"-dialog:
	QMessageBox confirmmsg( QMessageBox::Question, tr("Continue ?"), tr("Do you really want to reset all adjustment values ?"), QMessageBox::NoButton, this);
	confirmmsg.addButton(tr("OK"), QMessageBox::AcceptRole);
	confirmmsg.addButton(tr("Cancel"), QMessageBox::RejectRole);
	QFont confirmmsgfont = confirmmsg.font();
	confirmmsgfont.setPixelSize(12); // 9pts
	confirmmsg.setFont( confirmmsgfont );
	confirmmsg.show();
	uc = confirmmsg.exec();
	confirmmsg.close();
	if (uc != QMessageBox::AcceptRole)
		return;
	// Wait-messagebox:
	FSSM_WaitMsgBox waitmsgbox(this, tr("Resetting all adjustment values... Please wait !   "));
	waitmsgbox.show();
	// Reset all adjustment values:
	for (k=0; k<_supportedAdjustments.size(); k++)
	{
		if (!_SSMPdev->setAdjustmentValue(k, _supportedAdjustments.at(k).rawDefault))
		{
			communicationError(tr("No or invalid answer from Control Unit."));
			return;
		}
	}
	// Scale raw default values and display them:
	QString scaledValueString = "";
	for (k=0; k<_supportedAdjustments.size(); k++)
	{
		if (!raw2scaled(_supportedAdjustments.at(k).rawDefault, _supportedAdjustments.at(k).formula, _supportedAdjustments.at(k).precision, &scaledValueString))
			calcerror = true;
		displayCurrentValue(k, scaledValueString, _supportedAdjustments.at(k).unit);
	}
	// Close wait-messagebox:
	waitmsgbox.close();
	// Check for calculation error(s):
	if (calcerror)
		calculationError(tr("One or more values couldn't be scaled."));
}



bool CUcontent_Adjustments::raw2scaled(unsigned int rawValue, QString scaleformula, char precision, QString *scaledValueStr)
{
	bool success = false;
	double scaledValue = 0;
	if ( scaleformula.contains('=') )
	{
		success = raw2scaledByDirectAssociation(rawValue, scaleformula, scaledValueStr);
	}
	else if ( raw2scaledByCalculation( rawValue, scaleformula, &scaledValue ) )
	{
		*scaledValueStr = QString::number(scaledValue, 'f', precision);
		success = true;
	}
	else
		return false;
	return success;
}



bool CUcontent_Adjustments::raw2scaledByCalculation(unsigned int rawValue, QString scaleformula, double *scaledValue)
{
	if (scaleformula.size() < 1) return false;
	*scaledValue = rawValue;
	if (scaleformula.startsWith("MSB", Qt::CaseInsensitive))	// "special" calculation method...
	{
		// GET BASIC SCALE FACTOR:
		double factor = scaleformula.section(':',1,1).toDouble();
		// CHECK IF FACTOR IS VALID:
		if (factor == 0) return false;
		// GET DEFINDED MSB (Most Significant Bit):
		 // step 1: get string on the left side of ':'
		 // step 2: elimninate first three charcters ("MSB")
		 // step 3: convert to integer
		int msb = scaleformula.section(':',0,0).mid(3).toInt();
		// CHECK IF MSB DEFINITION IS VALID:
		if ((msb != 8) && (msb != 16)) return false;
		// SCALE RAW VALUE:
		if (rawValue > pow(2,msb-1) - 1)	// if MSB is set
		{
			*scaledValue = rawValue - pow(2, msb); // substract (half of range + 1)
		}
		*scaledValue = (*scaledValue) * factor;	// scale with basic factor
	}
	else	// "normal" formula
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
				ok = true; // Ok for putting this operator on the list
				if (nrofoperators > 0) // if this is not the first operator
				{
					// Check for consectuive operators
					if (((charindex - operatorindex[nrofoperators-1]) < 2))
						ok = false;
					// The 2nd operator will be interpreted as prefix duricng conversion to double...
				}
				if (ok) // if operator should be put on the list
				{
					operatorindex[nrofoperators] = charindex;
					nrofoperators++;
				}
				/* NOTE: No further checks necessary, conversion to double will fail
				*       - if we have more then two consecutive operators
				*       - or if the second operator is not a + or - (prefix)
				*/
			}
			else if (charindex == 0)	// if 1st character is not an operator
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
			// EXTRACT VALUE STRING AND VONVERT TO DOUBLE:
			tmpvaluestr = scaleformula.mid( operatorindex[calcstep]+1, tmpvaluestrlen );
			tmpvalue = tmpvaluestr.toDouble( &ok );
			if (!ok) return false;
			if (tmpvalue != 0)	// makes no sense, but we tolerate it...
			{
				// DO CALCUALTION STEP:
				switch (opchar)
				{
					case '+':
						*scaledValue += tmpvalue;
						break;
					case '-':
						*scaledValue -= tmpvalue;
						break;
					case '*':
						*scaledValue *= tmpvalue;
						break;
					case '/':
						*scaledValue /= tmpvalue;
						break;
					default:	// Error
						*scaledValue = 0;
						return false;
				}
			}
		}
	}
	return true;
}



bool CUcontent_Adjustments::raw2scaledByDirectAssociation(unsigned int rawValue, QString scaleformula, QString *scaledValueStr)
{
	unsigned int nrofDAs = 0;
	QString defstr = "";
	QString rvstr = "";
	bool da_success = false;
	unsigned int m = 0;

	nrofDAs = 1 + (scaleformula.count(','));
	for (m=0; m<nrofDAs; m++)
	{
		// Get next allocation string:
		defstr = scaleformula.section(',',m,m);
		// Get scaled value part of allocation string:
		rvstr = defstr.section('=', 0, 0);
		// Check if raw value matches allocation string:
		if ( rawValue == rvstr.toUInt() )
		{
			// Get allocated value:
			(*scaledValueStr) = defstr.section('=', 1, 1);
			da_success = true;
			break;
		}
	}
	return da_success;
}



bool CUcontent_Adjustments::scaled2raw(QString scaledValueStr, QString scaleformula, unsigned int *rawValue)
{
	bool success = false;
	double scaledValue = 0;
	if ( scaleformula.contains('=') )
	{
		success = scaled2rawByDirectAssociation(scaledValueStr, scaleformula, rawValue);
	}
	else
	{
		scaledValue = scaledValueStr.toDouble(&success);
		if (success)
			success = scaled2rawByCalculation(scaledValue, scaleformula, rawValue);
	}
	return success;
}



bool CUcontent_Adjustments::scaled2rawByDirectAssociation(QString scaledValueStr, QString scaleformula, unsigned int *rawValue)
{
	unsigned int nrofDAs = 0;
	QString defstr = "";
	QString svstr = "";
	bool ok = false;
	unsigned int m = 0;

	nrofDAs = 1 + (scaleformula.count(','));
	for (m=0; m<nrofDAs; m++)
	{
		// Get next allocation string:
		defstr = scaleformula.section(',',m,m);
		// Get scaled value part of allocation string:
		svstr = defstr.section('=', 1, 1);
		// Check if raw value matches allocation string:
		if ( scaledValueStr == svstr )
		{
			// Get allocated value:
			(*rawValue) = defstr.section('=', 0, 0).toUInt(&ok, 10);
			break;
		}
	}
	return ok;
}



bool CUcontent_Adjustments::scaled2rawByCalculation(double scaledValue, QString scaleformula, unsigned int *rawValue)
{
	double wval = scaledValue;
	if (scaleformula.size() < 1) return false;
	if (scaleformula.startsWith("MSB", Qt::CaseInsensitive))	// "special" calculation method...
	{
		// GET BASIC SCALE FACTOR:
		double factor = scaleformula.section(':',1,1).toDouble();
		// CHECK IF FACTOR IS VALID:
		if (factor == 0) return false;
		// GET DEFINDED MSB (Most Significant Bit):
		 // step 1: get string on the left side of ':'
		 // step 2: elimninate first three charcters ("MSB")
		 // step 3: convert to integer
		int msb = scaleformula.section(':',0,0).mid(3).toInt();
		// CHECK IF MSB DEFINITION IS VALID:
		if ((msb != 8) && (msb != 16)) return false;
		// CONVERT TO RAW VALUE:
		wval = wval / factor;	// REVERSE SCALE WITH BASIC FACTOR
		if (wval < 0)
		{
			wval += pow(2, msb);
		}
	}
	else	// "normal" formula
	{
		unsigned char tmpvaluestrlen = 0;
		QString tmpvaluestr("");
		double tmpvalue = 0;
		char opchar = 0;
		unsigned char operatorindex[10] = {0,};
		int nrofoperators = 0;
		int calcstep = 0;
		unsigned char charindex = 0;
		bool ok = false;
		// CHECK FORMULA AND GET OPERATOR POSITIONS:
		for (charindex=0; charindex<scaleformula.size(); charindex++)
		{
			if ((scaleformula.at(charindex) == '+') || (scaleformula.at(charindex) == '-') || (scaleformula.at(charindex) == '*') || (scaleformula.at(charindex) == '/'))
			{
				ok = true; // Ok for putting this operator on the list
				if (nrofoperators > 0) // if this is not the first operator
				{
					// Check for consectuive operators
					if (((charindex - operatorindex[nrofoperators-1]) < 2))
						ok = false;
					// The 2nd operator will be interpreted as prefix during conversion to double...
				}
				if (ok) // if operator should be put on the list
				{
					operatorindex[nrofoperators] = charindex;
					nrofoperators++;
				}
				/* NOTE: No further checks necessary, conversion to double will fail
				*       - if we have more then two consecutive operators
				*       - or if the second operator is not a + or - (prefix)
				*/
			}
			else if (charindex == 0)	// if 1st character is not an operator
			{
				return false;
			}
			else if (!scaleformula.at(charindex).isDigit())
			{
				return false;
			}
		}
		// CALCULATION:
		for (calcstep=(nrofoperators-1); calcstep>=0; calcstep--)    // CALCULATION STEP LOOP
		{
			// EXTRACT NEXT OPERATOR:
			opchar = scaleformula.at( (operatorindex[calcstep]) ).toAscii();
			// GET LENGTH OF VALUE STRING:
			if (calcstep >= (nrofoperators-1))	// IF LAST OPERATION/OPERATOR
				tmpvaluestrlen = scaleformula.size() - operatorindex[calcstep] -1;
			else
				tmpvaluestrlen = operatorindex[calcstep+1] - operatorindex[calcstep] -1;
			// CHECK VALUE STRING LENGTH:
			if (tmpvaluestrlen == 0)
				 return false;
			// EXTRACT VALUE STRING AND VONVERT TO DOUBLE:
			tmpvaluestr = scaleformula.mid( operatorindex[calcstep]+1, tmpvaluestrlen );
			tmpvalue = tmpvaluestr.toDouble( &ok );
			if (!ok) return false;
			if (tmpvalue != 0)	// makes no sense, but we tolerate it...
			{
				// DO CALCUALTION STEP (USE INVERSE OPERATOR !!!):
				switch (opchar)
				{
					case '+':
						wval -= tmpvalue;
						break;
					case '-':
						wval += tmpvalue;
						break;
					case '*':
						wval /= tmpvalue;
						break;
					case '/':
						wval *= tmpvalue;
						break;
					default:	// Error
						return false;
				}
			}
		}
	}
	wval = round(wval);
	if ((wval < 0) || (wval > 65535))
		return false;
	*rawValue = static_cast<unsigned int>(wval);
	return true;
}



void CUcontent_Adjustments::resizeEvent(QResizeEvent *event)
{
	int rowheight = 0;
	int vspace = 0;
	QHeaderView *headerview;
	unsigned int minnrofrows = 0;
	const char vspace_offset = -4;	// experimental value
	// Get available vertical space (for rows) and height per row:
	if (adjustments_tableWidget->rowCount() < 1)
		adjustments_tableWidget->setRowCount(1); // temporary create a row to get the row hight
	rowheight = adjustments_tableWidget->rowHeight(0);
	headerview = adjustments_tableWidget->horizontalHeader();
	vspace = adjustments_tableWidget->height() - headerview->height() + vspace_offset;
	// Temporary switch to "Scroll per Pixel"-mode to ensure auto-scroll (prevent white space between bottom of the last row and the lower table border)
	adjustments_tableWidget->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
	// Calculate and set nr. of rows:
	_maxrowsvisible = static_cast<unsigned int>(trunc((vspace-1)/rowheight) + 1);
	if (_maxrowsvisible < _supportedAdjustments.size())
		minnrofrows = _supportedAdjustments.size();
	else
		minnrofrows = _maxrowsvisible;
	adjustments_tableWidget->setRowCount(minnrofrows);
	// Set vertical scroll bar policy:
	if (minnrofrows > _supportedAdjustments.size())
		adjustments_tableWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	else
		adjustments_tableWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	// Switch back to "Scroll per Item"-mode:
	adjustments_tableWidget->setVerticalScrollMode( QAbstractItemView::ScrollPerItem ); // auto-scroll is triggered; Maybe this is a Qt-Bug, we don't want that here...
	// Accept event:
	event->accept();
}



bool CUcontent_Adjustments::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == adjustments_tableWidget->viewport())
	{
		if (event->type() == QEvent::Wheel)
		{
			if (adjustments_tableWidget->verticalScrollBarPolicy() ==  Qt::ScrollBarAlwaysOff)
			// ...or minnrofrows > _nrofsupportedAdjustments
				return true;	// filter out
			else
				return false;
		}
	}
	// pass the event on to the parent class
	return QWidget::eventFilter(obj, event);
}



void CUcontent_Adjustments::communicationError(QString errstr)
{
	errstr = tr("Communication Error:") + ('\n') + errstr;
	errorMsg(tr("Communication Error"), errstr);
	emit communicationError();
}



void CUcontent_Adjustments::calculationError(QString errstr)
{
	errstr = tr("Calculation Error:") + ('\n') + errstr;
	errorMsg(tr("Calculation Error"), errstr);
	emit calculationError();
}



void CUcontent_Adjustments::errorMsg(QString title, QString errstr)
{
	QMessageBox msg( QMessageBox::Critical, title, errstr, QMessageBox::Ok, this);
	QFont msgfont = msg.font();
	msgfont.setPixelSize(12); // 9pts
	msg.setFont( msgfont );
	msg.show();
	msg.exec();
	msg.close();
}



void CUcontent_Adjustments::setupUiFonts()
{
	// SET FONT FAMILY AND FONT SIZE
	// OVERWRITES SETTINGS OF ui_FreeSSM.h (made with QDesigner)
	QFont contentfont = QApplication::font();
	contentfont.setPixelSize(12);// 9pts
	contentfont.setBold(false);
	this->setFont(contentfont);
	adjustments_tableWidget->setFont(contentfont);
	nonPermanentInfo_label->setFont(contentfont);
}

