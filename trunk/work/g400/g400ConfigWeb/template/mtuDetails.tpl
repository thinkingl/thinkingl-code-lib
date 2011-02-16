<p class="title">转码规则</p>
<div id="mturules"></div>
<div><p>
<form id="form4" name="form4" enctype="multipart/form-data" method="post" action="kedacomfile">
<input type="button" name="Submit" class="leftFloat mainButton  mainButton_left" id="rule_import" onmousemove="myfile.style.pixelLeft=event.x-60;myfile.style.pixelTop=event.y-8+document.documentElement.scrollTop+ document.body.scrollTop;" value="导入..." />  
<input type="file" name="file" id="myfile"  size="1" onchange="form4_submit();" style="position:absolute;left:0px;top:0px;filter:alpha(opacity=0);" hidefocus/>
</form></p></div>
<input type="button" class="leftFloat mainButton" id="rule_extport" value="导出..." />
<input type="button" name="Submit3" class="leftFloat mainButton" id="rule_apply" value="保存" />
<input type="button" name="Submit2" class="rightFloat mainButton" id="rule_delete" value="删除" />                    
<input type="button" name="Submit" class="rightFloat mainButton" id="rule_alert" value="修改" />
<input type="button" name="Submit" class="rightFloat mainButton" id="rule_add" value="添加" />
                  
<a target="_blank" href="download/rules.ini" id="downrule" style="display:none">rule</a>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p class="title" style="margin-bottom:8px">转码服务器列表</p>
<div id="mtulist"></div> 
</p>
<input type="button" name="nmMtuEdit" class="rightFloat mainButton"  id="idMtuEdit" value="编辑"/>
<input type="button" name="nmMtuStop" class="rightFloat mainButton"  id="idMtuStop" value="停止"/>
<input type="button" name="nmMtuStart" class="rightFloat mainButton"  id="idMtuStart" value="启动"/>

