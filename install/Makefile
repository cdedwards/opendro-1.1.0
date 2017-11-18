install: install32 install64

RELEASE=1.1.0
WIXDIR="C:\Program Files (x86)\Windows Installer XML v3.5\bin"

opendro.wixobj: opendro.wxs
	candle.exe -arch x86 -out $@ -ext $(WIXDIR)\\WixUIExtension.dll -ext $(WIXDIR)\\WixDifxAppExtension.dll $<

opendro64.wixobj: opendro.wxs
	candle.exe -arch x64 -out $@ -ext $(WIXDIR)\\WixUIExtension.dll -ext $(WIXDIR)\\WixDifxAppExtension.dll $<

opendro-$(RELEASE).msi: opendro.wixobj
	light.exe -cultures:null -out $@ -pdbout opendro.wixpdb -sice:ICE57 -ext $(WIXDIR)\\WixUIExtension.dll -ext $(WIXDIR)\\WixDifxAppExtension.dll $(WIXDIR)\\difxapp_x86.wixlib $<

opendro64-$(RELEASE).msi: opendro64.wixobj
	light.exe -cultures:null -out $@ -pdbout opendro64.wixpdb -sice:ICE57 -ext $(WIXDIR)\\WixUIExtension.dll -ext $(WIXDIR)\\WixDifxAppExtension.dll $(WIXDIR)\\difxapp_x64.wixlib $<

install32: opendro-$(RELEASE).msi

install64: opendro64-$(RELEASE).msi

clean: clean32 clean64

clean32:
	rm -f opendro-$(RELEASE).msi opendro.wixobj opendro.wixobj opendro.wixpdb
    
clean64:
	rm -f opendro64-$(RELEASE).msi opendro64.wixobj opendro64.wixobj opendro64.wixpdb
