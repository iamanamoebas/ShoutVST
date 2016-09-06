**ShoutVST** is a VST that enables streaming sound into Icecast directly from VST hosts, ideal for streaming live performances directly from applications like **Traktor** or **Ableton** without the use of loopback methods. Only Windows is supported at the moment. 
You can try that precompiled binary (Just copy it into your VST plugins folder):

<a href="http://jsound.org/dl/ShoutVST.zip">
<img src="http://jsound.org/img/download.jpg" alt="ShoutVST Download" height="175" width="350">
</a>

![ShoutVST](http://jsound.org/img/ShoutVST.png "ShoutVST")

#Prerequisites#

[Visual Studio 2015](https://www.visualstudio.com/downloads/download-visual-studio-vs)

[CMake >=3.0](https://cmake.org/download/)

[GIT](https://git-scm.com/download/win)

#Build#

```
git clone https://github.com/R-Tur/ShoutVST.git
```

```
cd ShoutVST
```

```
!!make_project!!.bat
```

In order to re-obtain all dependencies, please refer to 
```
ShoutVST/deps/sync.bat
```

#The Result#

After successfull build the resulting DLL file will be available there (you can copy it into your VST folder):
```
ShoutVST\ShoutVST_DLL
```

#Tested Hosts#

Cubase (5.1.1), Reaper (5.22), FL Studio (11.1.0), LMMS (1.1.3)
