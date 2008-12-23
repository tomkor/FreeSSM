/*
 * FreeSSM.cpp - Program main window
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

#include "FreeSSM.h"



FreeSSM::FreeSSM(QApplication *app)
{
	_progversion = FSSM_VERSION;
	_qt_translator = NULL;
	_translator = NULL;
	_port = NULL;
	_SSMPdev = NULL;
	_portname = "";
	_language = "";
	_dumping = false;
	QString appsPath( QCoreApplication::applicationDirPath() );
	// SETUP GUI:
	setupUi(this);
	setupUiFonts();
	// LOAD BACKGROUND PICTURE:
	background_label->setPixmap(appsPath + "/background.jpg");
	// SHOW PROGRAM TITEL + VERSION:
	QFont titlefont = this->font();
	titlefont.setPixelSize(27); // 20pts
	titlefont.setBold(true);
	_progtitle_label = new QLabel(this);
	_progtitle_label->setGeometry( 20, 17, 315, 34);
	_progtitle_label->setFont( titlefont );
	_progtitle_label->setText("FreeSSM " + _progversion);
	this->setWindowTitle("FreeSSM " + _progversion);
	// PLACE WINDOW IN THE CENTER OF THE SCREEN:
	QDesktopWidget desktop;
	int screenWidth, width;
	int screenHeight, height;
	int x, y;
	// Get desktop size:
	QSize windowSize;
	screenWidth = desktop.width();		// get screen width
	screenHeight = desktop.height();	// get screen height
	// Get window size:
	windowSize = size(); // size of our application window
	width = windowSize.width(); 
	height = windowSize.height();
	// Calculate new window cordinates
	x = (screenWidth - width) / 2;
	y = (screenHeight - height) / 2;
	y -= 50;
	// Move window to desired coordinates
	this->move ( x, y );
	// LOAD PREFERENCES FROM FILE:
	QString savedportname = "";
	QString savedlanguage = "";
	QFile prefsfile(QDir::homePath() + "/FreeSSM.prefs");
	if (prefsfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray line;
		if (!prefsfile.atEnd())
		{
			// Load port settings:
			line = prefsfile.readLine();
			line.truncate(line.length()-1);	// truncate newline-character
			savedportname = static_cast<QString>(line);
		}
		if (!prefsfile.atEnd())
		{
			// Load language settings:
			line = prefsfile.readLine();
			line.truncate(line.length()-1);	// truncate newline-character
			savedlanguage = static_cast<QString>(line);
		}
		prefsfile.close();
	}
	// GET AVAILABLE PORTS:
	std::vector<std::string> portlist;
	portlist = serialCOM::GetAvailablePorts();
	// CHECK SAVED PREFERENCES AND CORRECT IF NECESSARY:
	bool pnvalid = false;
	for (unsigned int k=0; k<portlist.size(); k++)
	{
		if (savedportname == portlist[k].c_str())
		{
			pnvalid = true;
			break;
		}
	}
	if (pnvalid == true)
		_portname = savedportname;
	else
	{
		if (portlist.size()>0)
			_portname = (QString)portlist[0].c_str();
		// otherwise portname remains empty
	}
	// CHECK SAVED LANGUAGE AND CORRECT IF NECESSARY:
	if ((savedlanguage == "en") | (savedlanguage == "de"))
		_language = savedlanguage;
	else
	{
		// TRY TO GET SYSTEM LANGUAGE:
		if ((QLocale::system().language() == QLocale::English))
		{
			_language = "en";
		}
		else if ((QLocale::system().language() == QLocale::German))
		{
			_language = "de";
		}
		else	// if system language is not available/supported
		{
			_language = "en";
		}
	}
	// SET TRANSLATOR AND RETRANSLATE:
	_translator = new QTranslator;
	bool langfileerror = false;
	if (_language == "de")
	{
		langfileerror = !_translator->load("FreeSSM_de.qm", appsPath);
	}
	if ((_language == "en") | (langfileerror))
	{
		_language = "en";
		langfileerror = !_translator->load("FreeSSM_en.qm", appsPath);
	}
	if (!langfileerror)
	{
		app->installTranslator(_translator);
		retranslateUi(this);
	}
	else
	{
		delete _translator;
		_translator = NULL;
		QMessageBox msg( QMessageBox::Critical, tr("Error"), tr("Error:\n- Language file missing or damaged -"), QMessageBox::Ok, this);
		QFont msgfont = msg.font();
		msgfont.setPixelSize(12); // 9pts
		msg.setFont( msgfont );
		msg.show();
		msg.exec();
		msg.close();
	}
	// SET Qt-TRANSLATOR (if necessary and available):
	QString translationpath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	if (_language == "de")
	{
		_qt_translator = new QTranslator;
		if (_qt_translator->load("qt_de_DE", translationpath))
			app->installTranslator(_qt_translator);
		else
		{
			delete _qt_translator;
			_qt_translator = NULL;
		}
	}
	// CREATE ACTION FOR DUMPING CONTROL UNID ID-DATA TO FILE:
	QAction *dump_action = new QAction(this);
	dump_action->setShortcut( QKeySequence("Ctrl+Alt+Return") );
	this->addAction(dump_action);
	// CONNECT SIGNALS/SLOTS:
	connect( engine_pushButton, SIGNAL( pressed() ), this, SLOT( engine() ) );
	connect( transmission_pushButton, SIGNAL( pressed() ), this, SLOT( transmission() ) );
	connect( preferences_pushButton, SIGNAL( pressed() ), this, SLOT( preferences() ) );
	connect( help_pushButton, SIGNAL( pressed() ), this, SLOT( help() ) );
	connect( about_pushButton, SIGNAL( pressed() ), this, SLOT( about() ) );
	connect( exit_pushButton, SIGNAL( pressed() ), this, SLOT( close() ) );
	connect( dump_action, SIGNAL(triggered()), this, SLOT(dumpCUdata()) );
}


FreeSSM::~FreeSSM()
{
	disconnect( engine_pushButton, SIGNAL( pressed() ), this, SLOT( engine() ) ); 
	disconnect( transmission_pushButton, SIGNAL( pressed() ), this, SLOT( transmission() ) ); 
	disconnect( preferences_pushButton, SIGNAL( pressed() ), this, SLOT( preferences() ) ); 
	disconnect( help_pushButton, SIGNAL( pressed() ), this, SLOT( help() ) ); 
	disconnect( about_pushButton, SIGNAL( pressed() ), this, SLOT( about() ) ); 
	disconnect( exit_pushButton, SIGNAL( pressed() ), this, SLOT( close() ) );
	delete _progtitle_label;
	if (_port != NULL) delete _port;
	if (_SSMPdev != NULL) delete _SSMPdev;
	if (_translator != NULL)
	{
		QApplication::removeTranslator(_translator);
		delete _translator;
	}
	if (_qt_translator != NULL)
	{
		QApplication::removeTranslator(_qt_translator);
		delete _qt_translator;
	}
}


void FreeSSM::engine()
{
	_port = new serialCOM;
	if (initPort(4800, _port))
	{
		_SSMPdev = new SSMprotocol(_port, SSMprotocol::ECU, _language);
		Engine *enginewindow = new Engine(_SSMPdev, _progversion);
		connect(enginewindow, SIGNAL( destroyed() ), this, SLOT( SSMPdevCleanup() ));
	}
	else
	{
		delete _port;
		_port = NULL;
	}
}


void FreeSSM::transmission()
{
	_port = new serialCOM;
	if (initPort(4800, _port))
	{
		_SSMPdev = new SSMprotocol(_port, SSMprotocol::TCU, _language);
		Transmission *transmissionwindow = new Transmission(_SSMPdev, _progversion);
		connect(transmissionwindow, SIGNAL( destroyed() ), this, SLOT( SSMPdevCleanup() ));
	}
	else
	{
		delete _port;
		_port = NULL;
	}
}


void FreeSSM::preferences()
{
	Preferences *preferencesdlg = new Preferences(this, &_portname, _language);
	preferencesdlg->show();
	connect(preferencesdlg , SIGNAL( languageSelChanged(QString, QTranslator*) ),
					this, SLOT( retranslate(QString, QTranslator*) ));
	preferencesdlg->exec();
	delete preferencesdlg;
}


void FreeSSM::help()
{
	// Get working directory:
	QString helpurlstr( QCoreApplication::applicationDirPath() );
	// Select help file:
	if (_language == "de")
		helpurlstr += "/doc/help_de.html";
	else
		helpurlstr += "/doc/help_en.html";
	// Open help file in standard browswer:
	QDesktopServices::openUrl( QUrl(helpurlstr) );
}


void FreeSSM::about()
{
	About *aboutdlg = new About(this, _progversion, _language);
	aboutdlg->show();
	aboutdlg->exec();
	delete aboutdlg;
}


void FreeSSM::SSMPdevCleanup()
{
	disconnect(this, SLOT( SSMPdevCleanup() ));
	if (_SSMPdev)
	{
		delete _SSMPdev;
		_SSMPdev = NULL;
	}
	if (_port)
	{
		delete _port;	// port will be closed in destructor of serialCOM
		_port = NULL;
	}
}


bool FreeSSM::initPort(unsigned int baudrate, serialCOM *port)
{
	// IF NO PORT IS SELECTED YET: SELECT FIRST AVAILABLE PORT
	if (_portname == "")
	{
		// Get available ports:
		std::vector<std::string> portlist;
		portlist = serialCOM::GetAvailablePorts();
		if (portlist.size() > 0)
			_portname = (QString)portlist[0].c_str();
		else
		{
			QMessageBox msg( QMessageBox::Critical, tr("Error"),tr("No serial port available !"), QMessageBox::Ok, this);
			QFont msgfont = msg.font();
			msgfont.setPixelSize(12); // 9pts
			msg.setFont( msgfont );
			msg.show();
			msg.exec();
			msg.close();
			return false;
		}
	}
	// Open port:
	if (!port->OpenPort(_portname.toStdString()))
	{
		QMessageBox msg( QMessageBox::Critical, tr("Error"),tr("Couldn't open serial port !" "\n" "Maybe port is already in use by another application..."), QMessageBox::Ok, this);
		QFont msgfont = msg.font();
		msgfont.setPixelSize(12); // 9pts
		msg.setFont( msgfont );
		msg.show();
		msg.exec();
		msg.close();
		return false;
	}
	// Configure port:
	serialCOM::dt_portsettings newportsettings;
	newportsettings.baudrate = (double)baudrate;
	newportsettings.databits = 8;
	newportsettings.parity = 'N';
	newportsettings.stopbits = 1;
	if(!_port->SetPortSettings(newportsettings))
	{
		QMessageBox msg( QMessageBox::Critical, tr("Error"),tr("Couldn't apply the necessary serial port settings !"), QMessageBox::Ok, this);
		QFont msgfont = msg.font();
		msgfont.setPixelSize(12); // 9pts
		msg.setFont( msgfont );
		msg.show();
		msg.exec();
		msg.close();
		_port->ClosePort();
		return false;
	}
	return true;
}


void FreeSSM::retranslate(QString newlanguage, QTranslator *newtranslator)
{
	// Uninstall and delete current translator:
	if (_translator != NULL)
	{ 
		QApplication::removeTranslator(_translator);
		delete _translator; 
		_translator = NULL;
	}
	// Save new language settings:
	_language = newlanguage;
	_translator = newtranslator;
	// Install new translator and retranslate:
	QApplication::installTranslator(_translator);
	retranslateUi(this);
	// Change translator for Qt-dialogs:
	if (_qt_translator != NULL)
	{
		QApplication::removeTranslator(_qt_translator);
		delete _qt_translator;
		_qt_translator = NULL;
	}
	if (newlanguage=="de")
	{
		_qt_translator = new QTranslator();
		if ( _qt_translator->load("qt_de_DE", QLibraryInfo::location(QLibraryInfo::TranslationsPath)) )
			QApplication::installTranslator(_qt_translator);
		else
		{}	// Error
	}
}


void FreeSSM::dumpCUdata()
{
	QFile dumpfile(QCoreApplication::applicationDirPath() + "/dump.dat");
	char SYS_ID[3] = {0};
	char ROM_ID[5] = {0};
	char flagbytes[96] = {0};
	unsigned char nrofflagbytes = 0;
	QString hexstr = "";
	unsigned char k = 0;
	unsigned int dataaddr[17] = {0};
	char data[18] = {0};
	QString VIN = "";

	_dumping = true;
	disconnect( engine_pushButton, SIGNAL( pressed() ), this, SLOT( engine() ) );
	disconnect( transmission_pushButton, SIGNAL( pressed() ), this, SLOT( transmission() ) );
	disconnect( preferences_pushButton, SIGNAL( pressed() ), this, SLOT( preferences() ) );
	if (_port != NULL) std::cout << "Memory leak detected in FreeSSM::dumpCUdata() : port";
	_port = new serialCOM;
	// Initialize serial port:
	if (!initPort(4800, _port))
	{
		delete _port;	// port will be closed in destructor of serialCOM
		_port = NULL;
		connect( engine_pushButton, SIGNAL( pressed() ), this, SLOT( engine() ) );
		connect( transmission_pushButton, SIGNAL( pressed() ), this, SLOT( transmission() ) );
		connect( preferences_pushButton, SIGNAL( pressed() ), this, SLOT( preferences() ) );
		_dumping = false;
		return;
	}
	// Create SSMP-Communication-object:
	SSMPcommunication SSMPcom(_port, '\x10', 0);
	// *************** ECU ***************
	// Read ECU data:
	if (SSMPcom.getCUdata(SYS_ID, ROM_ID, flagbytes, &nrofflagbytes))
	{
		// Open/Create File:
		if (!dumpfile.open(QIODevice::WriteOnly | QIODevice::Text))
			goto end;
		// *** Convert and write data to file:
		// Sys-ID:
		StrToHexstr(SYS_ID, 3, &hexstr);
		dumpfile.write(hexstr.toAscii()+"\n", hexstr.length()+1);
		// ROM-ID:
		StrToHexstr(ROM_ID, 5, &hexstr);
		dumpfile.write(hexstr.toAscii()+"\n", hexstr.length()+1);
		// Flagbytes:
		for (k=1; k<=ceil(static_cast<float>(nrofflagbytes)/16); k++)
		{
			hexstr.clear();
			if (16*k <= nrofflagbytes)
				StrToHexstr(flagbytes+((k-1)*16), 16, &hexstr);
			else
				StrToHexstr(flagbytes+((k-1)*16), (nrofflagbytes%16), &hexstr);
			dumpfile.write(hexstr.toAscii()+"\n", hexstr.length()+1);
		}
		// VIN:
		if (flagbytes[36] == (flagbytes[36] | 0x01))
		{
			dataaddr[0] = 0xDA;
			dataaddr[1] = 0xDB;
			dataaddr[2] = 0xDC;
			if (!SSMPcom.readMultipleDatabytes(0x0, dataaddr, 3, data))
				goto end;
			StrToHexstr(data, 3, &hexstr);
			dumpfile.write("\n"+hexstr.toAscii()+"\n", hexstr.length()+2);
			dataaddr[0] = (65536 * static_cast<unsigned char>(data[0]))
					+ (256 * static_cast<unsigned char>(data[1]))
					+ (static_cast<unsigned char>(data[2]));
			for (k=1; k<17; k++)
				dataaddr[k] = dataaddr[0]+k;
			if (!SSMPcom.readMultipleDatabytes(0x0, dataaddr, 17, data))
				goto end;
			data[17] = '\x0';
			VIN = static_cast<QString>(data);
			dumpfile.write(VIN.toAscii()+"\n", 18);
		}
	}
	// *************** TCU ***************
	SSMPcom.setCUaddress('\x18');
	// Read TCU data:
	if (SSMPcom.getCUdata(SYS_ID, ROM_ID, flagbytes, &nrofflagbytes))
	{
		if (!dumpfile.isOpen()) // if file is not opened yet
		{
			// Open/Create File:
			if (!dumpfile.open(QIODevice::WriteOnly | QIODevice::Text))
				goto end;
		}
		// *** Convert and write data to file:
		// Sys-ID:
		StrToHexstr(SYS_ID, 3, &hexstr);
		dumpfile.write("\n"+hexstr.toAscii()+"\n", hexstr.length()+2);
		// ROM-ID:
		StrToHexstr(ROM_ID, 5, &hexstr);
		dumpfile.write(hexstr.toAscii()+"\n", hexstr.length()+1);
		// Flagbytes:
		for (k=1; k<=ceil(static_cast<float>(nrofflagbytes)/16); k++)
		{
			hexstr.clear();
			if (16*k <= nrofflagbytes)
				StrToHexstr(flagbytes+((k-1)*16), 16, &hexstr);
			else
				StrToHexstr(flagbytes+((k-1)*16), (nrofflagbytes%16), &hexstr);
			dumpfile.write(hexstr.toAscii()+"\n", hexstr.length()+1);
		}
	}
end:
	dumpfile.close();
	delete _port;	// port will be closed in destructor of serialCOM
	_port = NULL;
	connect( engine_pushButton, SIGNAL( pressed() ), this, SLOT( engine() ) ); 
	connect( transmission_pushButton, SIGNAL( pressed() ), this, SLOT( transmission() ) ); 
	connect( preferences_pushButton, SIGNAL( pressed() ), this, SLOT( preferences() ) ); 
	_dumping = false;
}


void FreeSSM::StrToHexstr(char *inputstr, unsigned int nrbytes, QString *hexstr)
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


void FreeSSM::keyPressEvent(QKeyEvent *event)
{
	QWidget *widget = NULL;
	QPushButton *pushbutton = NULL;
	if ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))
	{
		widget = this->focusWidget();
		pushbutton = dynamic_cast<QPushButton*>(widget);
		if (pushbutton)
			pushbutton->animateClick();
	}
	else if (event->key() == Qt::Key_Escape)
	{
		close();
	}
	else
		QMainWindow::keyPressEvent(event);
}


void FreeSSM::closeEvent(QCloseEvent *event)
{
	if (!_dumping)
		event->accept();
	else
		event->ignore();
}


void FreeSSM::setupUiFonts()
{
	// SET FONT FAMILY AND FONT SIZE
	// OVERWRITES SETTINGS OF ui_FreeSSM.h (made with QDesigner)
	QFont appfont = QApplication::font();
	QFont font = this->font();
	font.setFamily(appfont.family());
	font.setPixelSize(12);	// 9pts
	this->setFont(font);
	font = controlunits_groupBox->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	controlunits_groupBox->setFont(font);
	font = engine_pushButton->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	engine_pushButton->setFont(font);
	font = transmission_pushButton->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	transmission_pushButton->setFont(font);
	font = program_groupBox->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	program_groupBox->setFont(font);
	font = help_pushButton->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	help_pushButton->setFont(font);
	font = about_pushButton->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	about_pushButton->setFont(font);
	font = preferences_pushButton->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	preferences_pushButton->setFont(font);
	font = exit_pushButton->font();
	font.setFamily(appfont.family());
	font.setPixelSize(16);	// 12pts
	exit_pushButton->setFont(font);
}

