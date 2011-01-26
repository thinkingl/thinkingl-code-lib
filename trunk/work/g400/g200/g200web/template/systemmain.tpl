<table width="616" border="0">
										<tr>
														<td height="22" colspan="3" class="titleGroup">软件版本信息</td>
  </tr>
										
										<tr>
										  <td height="52" colspan="3" align="left" valign="top"><table width="100%" height="57" border="0" cellpadding="0" cellspacing="0">
                                            <tr>
                                              <td width="66%" height="28" class="lblTitlePrmt paddingLeft">当前版本：<span id="verInfo"></span></td>
                                              <td width="18%" align="right"><input name="reSetConfig" type="button" class="btIconNor"  value="恢复配置"  onclick="" style="visibility:hidden"/></td>
                                              <td width="16%" align="right"><input name="submitReboot" type="button" class="btIconNor" value="重新启动"  onclick="systemReboot();"/></td>
                                            </tr>
                                            <tr>
                                              <td height="23">&nbsp;</td>
                                              <td height="22">&nbsp;</td>
                                              <td>&nbsp;</td>
                                            </tr>
                                          </table></td>
  </tr>
										<tr>
														<td height="24" colspan="3" class="titleGroup">软件版本升级</td>
  </tr>
										<tr>
										  <td height="40" colspan="3" ><table width="100%" height="34" border="0" cellpadding="0" cellspacing="0">
  <tr valign="middle">
    <td width="66%" height="28" class="lblTitlePrmt paddingLeft">选择文件：
    <input type="text" name="fileName" size="45"/></td>
    <td width="34%"><form id="form1" name="form1" enctype='multipart/form-data' method="post" action="/kedacomfile" style="margin:0px;display:inline">
									                      <table width="100%" border="0" cellpadding="0" cellspacing="0">
                                                            <tr valign="middle">
                                                              <td width="52%" align="right" valign="middle"><input type='file' name='file' onChange='fileName.value=this.value' size='1' style='position:absolute; left:0px; top:0px; filter:alpha(opacity=0);' hidefocus />
                                                              <input type='button' name = 'selFile' onmousemove='file.style.pixelLeft=event.x-60;file.style.pixelTop=event.y-5+body.scrollTop;'  class='btIconNor' value='浏览...'/></td>
                                                              <td width="48%" align="right" valign="middle"><input type='button' name='updateSoftware' onClick="onSubmit(this.form);" class='btIconNor' value='版本升级' /></td>
                                                            </tr>
        </table>
    </form></td>
  </tr>
</table></td>
										</tr>
										<tr>
														<td height="24" colspan="3" class="lblTitlePrmt paddingLeft">升级说明：点击浏览，选择需要升级的软件包，点击&quot;版本升级&quot;按钮，文件开始上传。</td>
										</tr>
										<tr>
										  <td height="20" colspan="3">&nbsp;</td>
  </tr>
										<tr>
										  <td height="24" colspan="3" class="titleGroup">软件许可证</td>
  </tr>
										<tr>
										  <td height="24" colspan="3" ><table width="100%" height="191" border="0" cellpadding="0" cellspacing="0">
                                            <tr>
                                              <td width="116" height="28" align="right" class="lblTitlePrmt">许可证信息：</td>
                                              <td width="494" class="lblTitlePrmt"><span id="licenseInfo"></span></td>
                                            </tr>
                                            <tr>
                                              <td height="28" align="right" class="lblTitlePrmt">机器码：</td>
                                              <td class="lblTitlePrmt"><span id="mchKey"></span></td>
                                            </tr>
                                            <tr>
                                              <td height="28" align="right" class="lblTitlePrmt">许可证编号：</td>
                                              <td class="lblTitlePrmt"><span id="approval"></span></td>
                                            </tr>
                                            <tr>
                                              <td height="28" align="right" class="lblTitlePrmt">有效日期：</td>
                                              <td class="lblTitlePrmt"><span id="maxdate"></span></td>
                                            </tr>
                                            <tr>
                                              <td height="28" align="right" class="lblTitlePrmt">最大视频数：</td>
                                              <td class="lblTitlePrmt"><span id="videonum"></span></td>
                                            </tr>
                                            <tr>
                                              <td height="28" align="right" class="lblTitlePrmt">最大编码器通道数：</td>
                                              <td class="lblTitlePrmt"><span id="encodernum"></span></td>
                                            </tr>
                                            <tr>
                                              <td height="23" align="right">&nbsp;</td>
                                              <td>&nbsp;</td>
                                            </tr>
                                          </table></td>
  </tr>
										<tr>
														<td height="24" colspan="3" class="titleGroup">资源下载</td>
										</tr>
										<tr style="font:12px">
														<td height="37" colspan="3" class="paddingLeft"><a href="download/manual.rar">用户手册</a>&nbsp;&nbsp;<a href="download/VSHunterSetup.exe">设备搜索工具</a></td>
										</tr>
						</table>
