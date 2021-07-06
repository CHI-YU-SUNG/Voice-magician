## 聲音魔術師(Voice magician)
資訊流通的世代裡，訊息的隱私和安全性尤屬我們極為關切和注重的議題， 國家要利用科技獲得敵國軍事機密，有心人事不擇手段要竊取公司機密抑或重要的傳輸檔案，為了防止資訊被盜取利用，我們這次研究的目標著重在實現接收遠端傳輸加密後的聲音，並且此聲音是夾帶雜訊且人類無法直接辨別的音源，藉由我們一系列設計IOTDK的開發板，我們得以獲取解密後的音檔，達到秘密傳輸以及不被偷聽的目的。

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)
* [Demo Video](#demo-video)

## Introduction
這個裝置能夠將原本用高頻做聲音加密的訊號還原。

### Function
將聲音訊號的高頻部分濾除，留下含有效訊息的低頻部分，以達到解密效果。
### System Architecture
這個裝置包含了三個部分：接收、解密、輸出
- 接收：由RPi接收聲音檔，並將聲音資料傳給ARC
- 解密：ARC做消噪解密
- 輸出：解密後的音訊傳回RPi，再存回電腦播出

## Hardware and Software Setup
### Required Hardware
- 1 DesignWare ARC IoT Development Kit (IoTDK)
- 1 Raspberry Pi (RPi)
- 1 Notebook
### Required Software
- MobaXterm
- Digilent Adept software
### Hardware Connection
1. 本裝置會由電腦以無線網路的方式傳送音檔至RPi，再將音訊資料以有線串列傳輸，傳至ARC做處理後回傳至RPi，再存回電腦播放
	- 將RPi和電腦連上同一個WiFi
	- 將RPi的UART0_TXD與ARC的Arduino-IO0
	- 將RPi的UART0_RXD與ARC的Arduino-IO1
	- 將RPi的ground與ARC的Arduino-GND
	- RPi和ARC需共用電源
2. 裝置配置圖
	![alt tag](https://i.imgur.com/GLmFCfa.jpg)
	![alt tag](https://i.imgur.com/QO1cH0B.jpg)
	
## User Manual
### Before Running This Application
下載GitHub上的檔案
1. RPi:
	- rpi資料夾中的voiceprocess.py檔為python執行檔，請置於RPi中的任意處。建議存於桌面。
	- 將欲解密的音檔存放在跟voiceprocess.py同目錄下
2. notebook:
	- step1:clone https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git
	- step2:clone https://github.com/CHI-YU-SUNG/Voice-magician.git
	- step3:將"voice_magic"資料夾移至"embarc_osp/example/baremetal/"目錄下
### Run This Application
step1: notebook
	* 在voice_magic目錄下開啟terminal
	* 執行:make BOARD=iotdk TOOLCHAIN=gnu run
step2: RPi
	* 在voiceprocess.py所在目錄下執行:python voiceprocess.py
## Demo Video
[Youtube](https://youtu.be/6tQKfoc6kGY)
