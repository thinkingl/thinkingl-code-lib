﻿<p class='title'>软件版本信息</p>
<p><div class='rightTabCharater leftFloat'><span >当前版本:<span id='version_ver'></span></span></div>
          <input type='button' id='version_restart' name='Submit' class='rightFloat mainButton' value='重新启动' />
        </p>
        <p>&nbsp; </p>
		<p>&nbsp; </p>
        <p class='title'>软件版本升级</p><p>     
        <div>
          <table width="100%" height="36" border="0" cellpadding="0" cellspacing="0">
            <tr>
              <td width="30%" class="lblTitlePrmt" style="padding-left:9px">选择文件：    
              <td width="70%">
                <form id="form_g400_update" name="form1" enctype='multipart/form-data' method="post" action="/g400-xml/g400.fcgi?upload=/opt/uploadtest/g400test" style="margin:0px;display:inline">
                  
                  <table width="100%" height="21" border="0" cellpadding="0" cellspacing="0">
                    <tr valign="middle">
                      <td width="52%" height="21" align="right" valign="middle">                        
                        <input name="file" type='file' id='file_g400_update' />                        
                      </td>
                      <td width="48%" align="right" valign="middle">
                        <input type='button' id='btn_updateSoftware' class='mainButton' value='版本升级' style="margin-top:0px"/></td>
                    </tr>
                  </table>
                </form>
              </td>
            </tr>
          </table>
        </div>
          
        <p>升级说明:点击“浏览”按钮，选择升级软件包，点击“升级”按钮，开始升级。</p>
		<p>&nbsp; </p>
		<p class='title'>软件许可证</p>
		<div>
			<table width="100%" height="174" border="0" cellpadding="0" cellspacing="0">
				<tr>
				  <td width="308" height="29" align="right" class="lblTitlePrmt">许可证信息：</td>
				  <td width="765" class="lblTitlePrmt"><span id="licenseinfo"></span></td>
				  <td width="141" align="right" valign="bottom" class="lblTitlePrmt">
				  <form id="frmlicense" name="frmlicense" enctype='multipart/form-data' method="post" action="/kedacomconffile" style="margin:0px;display:inline">
            <input type='file' name='filelicense' id='filelicense'/>
            <input type="button" name="updatelicense" id="updatelicense" value="更新KEY" class="mainButton" onmousemove="filelicense.style.pixelLeft=event.x-80;filelicense.style.pixelTop=event.y-5 + body.scrollTop;" style="margin-top:0px"/>
				  </form>				  
				  </td>
				</tr>
				<tr>
				  <td height="28" align="right" class="lblTitlePrmt">机器码：</td>
				  <td colspan="2" class="lblTitlePrmt"><span id="mchkey"></span></td>
				</tr>
				<tr>
				  <td height="28" align="right" class="lblTitlePrmt">许可证编号：</td>
				  <td colspan="2" class="lblTitlePrmt"><span id="approval"></span></td>
				</tr>
				<tr>
				  <td height="28" align="right" class="lblTitlePrmt">有效日期：</td>
				  <td colspan="2" class="lblTitlePrmt"><span id="maxdate"></span></td>
				</tr>
		
			 </table>
		</div>
		<p>&nbsp; </p>
        <p class='title'>资源下载</p>
        <p style="font:12px">
          <a href='download/G400manual.rar'>用户手册</a> &nbsp;
          <a href='download/VSHunterSetup.exe'>设备搜索工具</a>&nbsp;
        </p>
        <p>&nbsp; </p>
