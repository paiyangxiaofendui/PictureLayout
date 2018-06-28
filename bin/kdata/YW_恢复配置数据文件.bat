set date=%date:~0,4%%date:~5,2%%date:~8,2%
if %time:~0,2% leq 9 (set hour=0%time:~1,1%) else (set hour=%time:~0,2%)
set min=%time:~3,2%%time:~6,2%
set DST=%date%%hour%%min%




::注意填写备份文件夹名称  中文+时间
#set Release8=YW_雕刻机配置文件备份(%DST%)
set Release8=YW_雕刻机配置文件备份

::在这里创建文件夹
#mkdir %Release8%  

#mkdir %Release8%\ico
#mkdir %Release8%\kdata 
#mkdir %Release8%\pdata
#mkdir %Release8%\picture
#mkdir %Release8%\report




::注意填写要备份的文件

::基本信息文件 运行目录
copy  "%Release8%\Knife.ini" 						".\"	

::kdata下的配置文件 刀库文件
copy  	"%Release8%\kdata\Excel.ywe"				"kdata"
copy  	"%Release8%\kdata\knifetype.ywe"			"kdata"
copy   	"%Release8%\kdata\leftpanel.ywe"			"kdata"
copy   	"%Release8%\kdata\Op.ywe"					"kdata"
copy   	"%Release8%\kdata\printcode.ywe"			"kdata"
copy   	"%Release8%\kdata\PrintModelSelect.ywe"		"kdata"
copy 	"%Release8%\kdata\SlotTemplate.ywe"			"kdata"
copy   	"%Release8%\kdata\UISetting.ywe"			"kdata"
copy  	"%Release8%\kdata\VecSolt.ywe"				"kdata"
copy   	"%Release8%\kdata\VecSoltChange.ywe"		"kdata"


::picture下的配置文件 
copy  	"%Release8%\picture\printcode.ywe"			"picture" 


::report下的配置文件
copy  	"%Release8%\report\lingliaobiao.xml"		"report" 			


::要保存什么文件按照上面的格式填写就可以了






