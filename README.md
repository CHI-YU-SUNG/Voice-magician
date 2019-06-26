## Voice magician
資訊流通的世代裡，訊息的隱私和安全性尤屬我們極為關切和注重的議題， 國家要利用科技獲得敵國軍事機密，有心人事不擇手段要竊取公司機密抑或重要的傳輸檔案，為了防止資訊被盜取利用，我們這次研究的目標著重在實現接收遠端傳輸加密後的聲音，並且此聲音是夾帶雜訊且人類無法直接辨別的音源，藉由我們一系列設計IOTDK的開發板，我們得以獲取解密後的音檔，達到秘密傳輸以及不被偷聽的目的。
* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#System-Architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

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
1. 本裝置會由電腦傳送音檔至RPi，再將音訊資料傳至ARC
	- 將RPi的電源線接至電腦的USB孔
	- 將RPi的UART0_TXD與ARC的I2S_RX
2. 裝置配置圖

## User Manual
### Before Running This Application
下載GitHub上的檔案至電腦中

- **粗體字**
- **Sleep monitoring** 一般字體
- Communicate with **APP**
- **Alarm Clock** Setting 
- **Music** based on Uart protocol (Supports file umlimited songs)
- **Alexa** Voice assistant, connecting and communicating with Wifi.
- **CPU Loading** Calculate the power used by the board
## Demo
//to do 
| Main page | Clock | Record |
| -------- | -------- | -------- |
| ![](https://imgur.com/gallery/YSJayCb)     
## Link
//to do 
