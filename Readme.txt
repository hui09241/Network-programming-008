本週練習三可以讓多個client隨時連入，但是當有client離開時，輪詢就可能發生錯誤。

請將程式增加「支援client隨時離開(斷線)與加入」的功能。client數上限為10。

提示：修改socket陣列當新增或減少client時。