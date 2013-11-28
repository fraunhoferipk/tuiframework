
To start SketchApp:

1. Execute batch file named "runSupercolliderServer.bat" on Desktop (also available at C:\git\tuiframework\tuiframework\cpp\tuilibs\sound)
2. Run tuiframework in Debug mode, within Visual Studio (equivalent to running "TUIBinTUIServer2.exe 7998 7999 SuperColliderConfig.xml . > debug.txt" in C:\git\tuiframework\tuiframework\cpp\Windows\Debug). Output is sent to C:\git\tuiframework\tuiframework\cpp\Windows\Debug\debug.txt, although this can be reconfigured in Visual Studio if the Debug settings are modified within the settings of the TUIBinTUIServer2 project.
3. Run mouse server.

To change test conditions, open C:\git\tuiframework\tuiframework\cpp\Windows\Debug\SuperColliderConfig.xml and edit this part:

<DeviceInstance name="OversketchSoundDevice" typename="SoundDevice">
	<ParameterGroup name="cfgfile">
		<Parameter name="Path" value="c:\git\tuiframework\tuiframework\cpp\tuilibs\sound\"/>
		<Parameter name="CfgFilename" value="friction.cfg"/>
		<Parameter name="GradationFilename" value="frictionSpeedGradations.cfg"/>
	</ParameterGroup>
</DeviceInstance>
			 
such that GradationFilename is set to one of the following three conditions:

frictionSpeedGradations.cfg (both friction and speed of strokes are modelled)
frictionGradations.cfg (only friction is taken into consideration, stroke speed is don't care)
speedGradations.cfg (only stroke speed is taken into consideration, friction is don't care)

