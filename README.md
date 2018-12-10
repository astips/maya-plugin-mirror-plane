# maya-plugin-mirror-plane
Maya custom locator node with a transparent shape. support viewport2.0 diaplay mode.


#### MAYA NODE ID
* Node ID : 0x71000


#### INSTALLATION
1. Download the latest release and unzip the folder where you want to live.
2. Copy icon file "out_mirrorPlane.png" into `%USERPROFILE%\Documents\maya\mayaVersion\prefs\icons`
3. Copy Plugin file "mirrorPlane.so(.mll)" into you maya's plugins folder.
4. Load plugin in maya, run these commands in Python to create mirrorPlane node.


```python
import maya.cmds as cmds
cmds.createNode("mirrorPlane")
```
