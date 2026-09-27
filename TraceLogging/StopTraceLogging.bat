@echo off
set toolpath="C:\Program Files (x86)\Windows Kits\10\Windows Performance Toolkit\"
@rem %toolpath%\wpr.exe -stop GeneralProfile -stop TraceCaptureFile.etl description
%toolpath%\wpr.exe -stop TraceCaptureFile.etl description