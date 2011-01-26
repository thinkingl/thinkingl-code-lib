<table width="100%" height="87" border="0">
  <tr>
    <td height="23" class="titleGroup">矩阵管理</td>
  </tr>
  <tr>
    <td height="58" valign="top"><div id="matrixlist"> </div>
        <div id="btnoperation">
          <table width="100%" height="36" border="0" cellpadding="0" cellspacing="0">
            <tr>
              <td width="81" height="27">
			  <form id="form1" name="form1" enctype='multipart/form-data' method="post" action="/kedacomconffile" style="margin:0px;display:inline">
			  <input type="button" name="import" value="导入..." class="btIconNor" onmousemove="file.style.pixelLeft=event.x-80;file.style.pixelTop=event.y-5 + body.scrollTop;"/>
			  <input type='file' name='file'  onchange="formsubmit(this.form);" size='1' style='position:absolute; left:0px; top:0px; filter:alpha(opacity=0);' hidefocus />
			  </form>			  </td>
              <td width="108">
			  <input type="button" name="export" value="导出..." class="btIconNor" onclick="exportFile(this.form);"/>
			  </td>
              <td width="173"><a href="download/conf.tar" id="hrefExport" style="visibility:hidden">导出</a></td>
              <td width="81" align="right"><input type="button" name="new" value="添加矩阵" class="btIconNor" onclick="addMatrix();"/></td>
              <td width="81" align="right"><input type="button" name="delete" value="删除矩阵" class="btIconNor"  onclick="deleteMatrix();"/></td>
              <td width="81" align="right"><input type="button" name="update" value="修改矩阵" class="btIconNor" onclick="updateMatrix();"/></td>
            </tr>
          </table>
    </div></td>
  </tr>
</table>
