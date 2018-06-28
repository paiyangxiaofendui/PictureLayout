set date=%date:~0,4%%date:~5,2%%date:~8,2%
if %time:~0,2% leq 9 (set hour=0%time:~1,1%) else (set hour=%time:~0,2%)
set min=%time:~3,2%%time:~6,2%
set DST=%date%%hour%%min%




::注意填写备份文件夹名称  中文+时间
#set Release8=雕刻机配置文件备份(%DST%)
set Release8=YW_雕刻机配置文件备份

::在这里创建文件夹
mkdir %Release8%  

::mkdir %Release8%\ico
mkdir %Release8%\kdata 
::mkdir %Release8%\pdata
mkdir %Release8%\picture
mkdir %Release8%\report




::注意填写要备份的文件

::基本信息文件 运行目录
copy  "Knife.ini" 							"%Release8%" 	

::kdata下的配置文件 刀库文件
copy  "kdata\Excel.ywe" 					"%Release8%\kdata"	
copy  "kdata\knifetype.ywe" 				"%Release8%\kdata"	
copy  "kdata\leftpanel.ywe" 				"%Release8%\kdata"	
copy  "kdata\Op.ywe" 						"%Release8%\kdata"
copy  "kdata\printcode.ywe" 				"%Release8%\kdata"	
copy  "kdata\PrintModelSelect.ywe" 			"%Release8%\kdata"	
copy  "kdata\SlotTemplate.ywe" 				"%Release8%\kdata"	
copy  "kdata\UISetting.ywe" 				"%Release8%\kdata"	
copy  "kdata\VecSolt.ywe" 					"%Release8%\kdata"	
copy  "kdata\VecSoltChange.ywe" 			"%Release8%\kdata"	


::picture下的配置文件 刀库文件
copy  "picture\printcode.ywe" 				"%Release8%\picture"	


::report下的配置文件 刀库文件
copy  "report\lingliaobiao.xml" 			"%Release8%\report"	


::要保存什么文件按照上面的格式填写就可以了






