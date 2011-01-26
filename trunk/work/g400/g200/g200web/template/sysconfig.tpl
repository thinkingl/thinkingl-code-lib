<table width="616">
								<tr>
									<td height="23" class="titleGroup">平台配置</td>
								</tr>
								<tr>
								  <td height="23" ><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                    <tr>
                                      <td height="28" align="right" class="lblTitlePrmt">连接平台：</td>
                                      <td height="28" align="left" class="lblTitlePrmt"><span id="mplatStatus"></span></td>
                                      <td colspan="2">&nbsp;</td>
                                    </tr>
                                    <tr>
                                      <td height="28" align="right" class="lblTitlePrmt">连接3A：</td>
                                      <td height="28" align="left" class="lblTitlePrmt"><span id="3aStatus"></span></td>
                                      <td colspan="2">&nbsp;</td>
                                    </tr>
                                    <tr>
                                      <td height="28" align="right" class="lblTitlePrmt">类型：                                      </td>
                                      <td height="28" align="left" class="lblTitlePrmt"><span id="mplattype"></span></td>
                                      <td colspan="2">&nbsp;</td>
                                    </tr>
                                    <tr>
                                      <td width="80" height="28" align="right" class="lblTitlePrmt">平台IP地址：</td>
                                      <td width="359" class="lblTitlePrmt"><span id="mplatip"></span>
												&nbsp;&nbsp;
												端口：
									  <span id="mplatport"></span></td>
                                      <td colspan="2">&nbsp;</td>
                                    </tr>
                                    <tr>
                                      <td height="28" align="right" class="lblTitlePrmt">3AIP地址：</td>
                                      <td class="lblTitlePrmt"><span id="ip3a"></span>
									  			&nbsp;&nbsp;
									  			端口：
									  <span id="port3a"></span></td>
                                      <td colspan="2">&nbsp;</td>
                                    </tr>
                                    <tr>
                                      <td height="28" align="right" class="lblTitlePrmt">3A用户名：</td>
                                      <td class="lblTitlePrmt"><span id="3auser"></span></td>
                                      <td colspan="2">&nbsp;</td>
                                    </tr>
                                    <tr>
                                      <td height="28" align="right" class="lblTitlePrmt">3A密码：</td>
                                      <td class="lblTitlePrmt"><span id="3apsw"></span></td>
                                      <td colspan="2">&nbsp;</td>
                                    </tr>
                                    <tr>
                                      <td height="28" align="right" class="lblTitlePrmt">设备入网ID：</td>
                                      <td class="lblTitlePrmt"><div id="No"></div></td>
                                      <td width="87" align="right"><input name="btAppSysConf" type="button" class="btIconNor" value="应用" style="visibility:hidden" onclick="saveSysConf()"/></td>
                                      <td width="82" align="right"><input name="btUpdateSysConf" type="button" class="btIconNor" value="修改" onclick="updateSysConf();"/></td>
                                    </tr>
                                    <tr>
                                      <td>&nbsp;</td>
                                      <td>&nbsp;</td>
                                      <td colspan="2" align="right">&nbsp;</td>
                                    </tr>
                                  </table></td>
  </tr>
				<tr>
								<td height="23" class="titleGroup">网关配置</td>
				</tr>
				
				<tr>
								<td><table width="100%" border="0" cellpadding="0" cellspacing="0">
                                  <tr>
                                    <td width="71" height="34" align="right" class="lblTitlePrmt">IP地址：</td>
                                    <td width="187" class="lblTitlePrmt"><div id="selIp"></div></td>
                                    <td width="268">&nbsp;</td>
                                    <td width="82">&nbsp;</td>
                                  </tr>
                                  <tr>
                                    <td width="71" height="34" align="right" class="lblTitlePrmt">子网掩码：</td>
                                    <td class="lblTitlePrmt"><div id="subMask"></div></td>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                  </tr>
                                  <tr>
                                    <td width="71" height="29" align="right" class="lblTitlePrmt">默认网关：</td>
                                    <td class="lblTitlePrmt"><div id="defaultGate"></div></td>
                                    <td align="right"><input name="btAppLocalIp" type="button" class="btIconNor" value="应用" style="visibility:hidden" onclick="savelocalIp()"/></td>
                                    <td align="right"><input name="btUpdateIp" type="button" class="btIconNor" value="修改" onclick="updateLocalIp()"/></td>
                                  </tr>
                                </table></td>
				</tr>
</table>
