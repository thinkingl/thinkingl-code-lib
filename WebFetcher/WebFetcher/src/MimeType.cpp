#include "..\include\MimeType.h"
#include "log.h"

struct TMimeType
{
	CMimeType::EMimeType m_eMimeType;
	tstring m_strContentTypeToken;
	tstring m_strFileExt;

	TMimeType(CMimeType::EMimeType eMimeType, LPCTSTR strContentTypeToken, LPCTSTR strFileExt )
	{
		this->m_eMimeType = eMimeType;
		this->m_strContentTypeToken = strContentTypeToken;
		this->m_strFileExt = strFileExt;
	}
};

static const TMimeType s_arMimeTypeDic[]=
{
	TMimeType( CMimeType::HttpMimeHtmlHtm, _T( "text/html" ), _T( "html" ) ),
	TMimeType( CMimeType::HttpMimeHtmlHtm, _T( "text/html" ), _T( "htm" ) ),
	TMimeType( CMimeType::HttpMimeHqx, _T( "application/mac-binhex40" ), _T( "hqx" ) ),
	TMimeType( CMimeType::HttpMimeCpt, _T( "xxxxxxxxxx" ), _T( "cpt" ) ),
	TMimeType( CMimeType::HttpMimeDoc, _T( "xxxxxxxxxx" ), _T( "doc" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "bin" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "dms" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "lha" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "lzh" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "exe" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "class" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "so" ) ),
	TMimeType( CMimeType::HttpMimeBinDmsLhaLzhExeClassSoDll, _T( "application/octet-stream" ), _T( "dll" ) ),
	TMimeType( CMimeType::HttpMimeOda, _T( "application/oda" ), _T( "oda" ) ),
	TMimeType( CMimeType::HttpMimePdf, _T( "application/pdf" ), _T( "pdf" ) ),
	TMimeType( CMimeType::HttpMimeAiEpsPs, _T( "application/postscript" ), _T( "ai" ) ),
	TMimeType( CMimeType::HttpMimeAiEpsPs, _T( "application/postscript" ), _T( "eps" ) ),
	TMimeType( CMimeType::HttpMimeAiEpsPs, _T( "application/postscript" ), _T( "ps" ) ),
	TMimeType( CMimeType::HttpMimeSmiSmil, _T( "application/smil" ), _T( "smi" ) ),
	TMimeType( CMimeType::HttpMimeSmiSmil, _T( "application/smil" ), _T( "smil" ) ),
	TMimeType( CMimeType::HttpMimeMif, _T( "application/vnd.mif" ), _T( "mif" ) ),
	TMimeType( CMimeType::HttpMimeXls, _T( "pplication/vnd.ms-excel" ), _T( "xls" ) ),
	TMimeType( CMimeType::HttpMimePpt, _T( "application/vnd.ms-powerpoint" ), _T( "ppt" ) ),
	TMimeType( CMimeType::HttpMimeWbxml, _T( "application/vnd.wap.wbxml" ), _T( "wbxml" ) ),
	TMimeType( CMimeType::HttpMimeWmlc, _T( "application/vnd.wap.wmlc" ), _T( "wmlc" ) ),
	TMimeType( CMimeType::HttpMimeWmlsc, _T( "application/vnd.wap.wmlscriptc" ), _T( "wmlsc" ) ),
	TMimeType( CMimeType::HttpMimeBcpio, _T( "application/x-bcpio" ), _T( "bcpio" ) ),
	TMimeType( CMimeType::HttpMimeVcd, _T( "application/x-cdlink" ), _T( "vcd" ) ),
	TMimeType( CMimeType::HttpMimePgn, _T( "application/x-chess-pgn" ), _T( "pgn" ) ),
	TMimeType( CMimeType::HttpMimeCpio, _T( "application/x-cpio" ), _T( "cpio" ) ),
	TMimeType( CMimeType::HttpMimeCsh, _T( "application/x-csh" ), _T( "csh" ) ),
	TMimeType( CMimeType::HttpMimeDcrDirDxr, _T( "application/x-director" ), _T( "dcr" ) ),
	TMimeType( CMimeType::HttpMimeDcrDirDxr, _T( "application/x-director" ), _T( "dir" ) ),
	TMimeType( CMimeType::HttpMimeDcrDirDxr, _T( "application/x-director" ), _T( "dxr" ) ),
	TMimeType( CMimeType::HttpMimeDvi, _T( "application/x-dvi" ), _T( "dvi" ) ),
	TMimeType( CMimeType::HttpMimeSpl, _T( "application/x-futuresplash" ), _T( "spl" ) ),
	TMimeType( CMimeType::HttpMimeGtar, _T( "application/x-gtar" ), _T( "gtar" ) ),
	TMimeType( CMimeType::HttpMimeHdf, _T( "application/x-hdf" ), _T( "hdf" ) ),
	TMimeType( CMimeType::HttpMimeJs, _T( "application/x-javascript" ), _T( "js" ) ),

	//	strContentType = 0x009eda78 "text/javascript; charset=utf-8"
	TMimeType( CMimeType::HttpMimeJs, _T( "text/javascript" ), _T( "js" ) ),

	TMimeType( CMimeType::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skp" ) ),
	TMimeType( CMimeType::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skd" ) ),
	TMimeType( CMimeType::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skt" ) ),
	TMimeType( CMimeType::HttpMimeSkpSkdSktSkm, _T( "application/x-koan" ), _T( "skm" ) ),
	TMimeType( CMimeType::HttpMimeLatex, _T( "application/x-latex" ), _T( "latex" ) ),
	TMimeType( CMimeType::HttpMimeNcCdf, _T( "application/x-netcdf" ), _T( "nc" ) ),
	TMimeType( CMimeType::HttpMimeNcCdf, _T( "application/x-netcdf" ), _T( "cdf" ) ),
	TMimeType( CMimeType::HttpMimeSh, _T( "application/x-sh" ), _T( "sh" ) ),
	TMimeType( CMimeType::HttpMimeShar, _T( "application/x-shar" ), _T( "shar" ) ),
	TMimeType( CMimeType::HttpMimeSwf, _T( "application/x-shockwave-flash" ), _T( "swf" ) ),
	TMimeType( CMimeType::HttpMimeSit, _T( "application/x-stuffit" ), _T( "sit" ) ),
	TMimeType( CMimeType::HttpMimeSv4cpio, _T( "application/x-sv4cpio" ), _T( "sv4cpio" ) ),
	TMimeType( CMimeType::HttpMimeSv4crc, _T( "application/x-sv4crc" ), _T( "sv4crc" ) ),
	TMimeType( CMimeType::HttpMimeTar, _T( "application/x-tar" ), _T( "tar" ) ),
	TMimeType( CMimeType::HttpMimeTcl, _T( "application/x-tcl" ), _T( "tcl" ) ),
	TMimeType( CMimeType::HttpMimeTex, _T( "application/x-tex" ), _T( "tex" ) ),
	TMimeType( CMimeType::HttpMimeTexinfoTexi, _T( "application/x-texinfo" ), _T( "texinfo" ) ),
	TMimeType( CMimeType::HttpMimeTexinfoTexi, _T( "application/x-texinfo" ), _T( "texi" ) ),
	TMimeType( CMimeType::HttpMimeT_TrRoff, _T( "application/x-troff" ), _T( "t" ) ),
	TMimeType( CMimeType::HttpMimeT_TrRoff, _T( "application/x-troff" ), _T( "tr" ) ),
	TMimeType( CMimeType::HttpMimeT_TrRoff, _T( "application/x-troff" ), _T( "roff" ) ),
	TMimeType( CMimeType::HttpMimeMan, _T( "application/x-troff-man" ), _T( "man" ) ),
	TMimeType( CMimeType::HttpMimeMe, _T( "application/x-troff-me" ), _T( "me" ) ),
	TMimeType( CMimeType::HttpMimeMs, _T( "application/x-troff-ms" ), _T( "ms" ) ),
	TMimeType( CMimeType::HttpMimeUstar, _T( "application/x-ustar" ), _T( "ustar" ) ),
	TMimeType( CMimeType::HttpMimeSrc, _T( "application/x-wais-source" ), _T( "src" ) ),
	TMimeType( CMimeType::HttpMimeXhtmlXht, _T( "application/xhtml+xml" ), _T( "xhtml" ) ),
	TMimeType( CMimeType::HttpMimeXhtmlXht, _T( "application/xhtml+xml" ), _T( "xht" ) ),
	TMimeType( CMimeType::HttpMimeZip, _T( "application/zip" ), _T( "zip" ) ),

	// strContentType = 0x04dc3fe0 "application/x-zip-compressed"
	TMimeType( CMimeType::HttpMimeZip, _T( "application/x-zip-compressed" ), _T( "zip" ) ),

	TMimeType( CMimeType::HttpMimeAuSnd, _T( "audio/basic" ), _T( "au" ) ),
	TMimeType( CMimeType::HttpMimeAuSnd, _T( "audio/basic" ), _T( "snd" ) ),
	TMimeType( CMimeType::HttpMimeMidMidiKar, _T( "audio/midi" ), _T( "mid" ) ),
	TMimeType( CMimeType::HttpMimeMidMidiKar, _T( "audio/midi" ), _T( "midi" ) ),
	TMimeType( CMimeType::HttpMimeMidMidiKar, _T( "audio/midi" ), _T( "kar" ) ),
	TMimeType( CMimeType::HttpMimeMpgaMp2Mp3, _T( "audio/mpeg" ), _T( "mpga" ) ),
	TMimeType( CMimeType::HttpMimeMpgaMp2Mp3, _T( "audio/mpeg" ), _T( "mp2" ) ),
	TMimeType( CMimeType::HttpMimeMpgaMp2Mp3, _T( "audio/mpeg" ), _T( "mp3" ) ),
	TMimeType( CMimeType::HttpMimeAifAiffAifc, _T( "audio/x-aiff" ), _T( "aif" ) ),
	TMimeType( CMimeType::HttpMimeAifAiffAifc, _T( "audio/x-aiff" ), _T( "aiff" ) ),
	TMimeType( CMimeType::HttpMimeAifAiffAifc, _T( "audio/x-aiff" ), _T( "aifc" ) ),
	TMimeType( CMimeType::HttpMimeM3u, _T( "audio/x-mpegurl" ), _T( "m3u" ) ),
	TMimeType( CMimeType::HttpMimeRamRm, _T( "audio/x-pn-realaudio" ), _T( "ram" ) ),
	TMimeType( CMimeType::HttpMimeRamRm, _T( "audio/x-pn-realaudio" ), _T( "rm" ) ),
	TMimeType( CMimeType::HttpMimeRpm, _T( "audio/x-pn-realaudio-plugin" ), _T( "rpm" ) ),
	TMimeType( CMimeType::HttpMimeRa, _T( "audio/x-realaudio" ), _T( "ra" ) ),
	TMimeType( CMimeType::HttpMimeWav, _T( "audio/x-wav" ), _T( "wav" ) ),
	TMimeType( CMimeType::HttpMimePdb, _T( "chemical/x-pdb" ), _T( "pdb" ) ),
	TMimeType( CMimeType::HttpMimeXyz, _T( "chemical/x-xyz" ), _T( "xyz" ) ),
	TMimeType( CMimeType::HttpMimeBmp, _T( "image/bmp" ), _T( "bmp" ) ),
	TMimeType( CMimeType::HttpMimeGif, _T( "image/gif" ), _T( "gif" ) ),
	TMimeType( CMimeType::HttpMimeIef, _T( "image/ief" ), _T( "ief" ) ),
	TMimeType( CMimeType::HttpMimeJpegJpgJpe, _T( "image/jpeg" ), _T( "jpeg" ) ),
	TMimeType( CMimeType::HttpMimeJpegJpgJpe, _T( "image/jpeg" ), _T( "jpg" ) ),
	TMimeType( CMimeType::HttpMimeJpegJpgJpe, _T( "image/jpeg" ), _T( "jpe" ) ),

	// strContentType = 0x027e17f0 "image/jpg"
	TMimeType( CMimeType::HttpMimeJpegJpgJpe, _T( "image/jpg" ), _T( "jpg" ) ),

	// strContentType = 0x0464fcd0 "httpd/yahoo-send-as-is"
//	TMimeType( CMimeType::HttpMimeJpegJpgJpe, _T( "httpd/yahoo-send-as-is" ), _T( "jpg" ) ),

	TMimeType( CMimeType::HttpMimePng, _T( "image/png" ), _T( "png" ) ),
	TMimeType( CMimeType::HttpMimeTiffTif, _T( "image/tiff" ), _T( "tiff" ) ),
	TMimeType( CMimeType::HttpMimeTiffTif, _T( "image/tiff" ), _T( "tif" ) ),
	TMimeType( CMimeType::HttpMimeDjvuDjv, _T( "image/vnd.djvu" ), _T( "djvu" ) ),
	TMimeType( CMimeType::HttpMimeDjvuDjv, _T( "image/vnd.djvu" ), _T( "djv" ) ),
	TMimeType( CMimeType::HttpMimeWbmp, _T( "image/vnd.wap.wbmp" ), _T( "wbmp" ) ),
	TMimeType( CMimeType::HttpMimeRas, _T( "image/x-cmu-raster" ), _T( "ras" ) ),
	TMimeType( CMimeType::HttpMimePnm, _T( "image/x-portable-anymap" ), _T( "pnm" ) ),
	TMimeType( CMimeType::HttpMimePbm, _T( "image/x-portable-bitmap" ), _T( "pbm" ) ),
	TMimeType( CMimeType::HttpMimePgm, _T( "image/x-portable-graymap" ), _T( "pgm" ) ),
	TMimeType( CMimeType::HttpMimePpm, _T( "image/x-portable-pixmap" ), _T( "ppm" ) ),
	TMimeType( CMimeType::HttpMimeRgb, _T( "image/x-rgb" ), _T( "rgb" ) ),
	TMimeType( CMimeType::HttpMimeXbmXpm, _T( "image/x-xpixmap" ), _T( "xbm" ) ),
	TMimeType( CMimeType::HttpMimeXbmXpm, _T( "image/x-xpixmap" ), _T( "xpm" ) ),
	TMimeType( CMimeType::HttpMimeXwd, _T( "image/x-xwindowdump" ), _T( "xwd" ) ),
	TMimeType( CMimeType::HttpMimeIgsIges, _T( "model/iges" ), _T( "igs" ) ),
	TMimeType( CMimeType::HttpMimeIgsIges, _T( "model/iges" ), _T( "iges" ) ),
	TMimeType( CMimeType::HttpMimeMshMeshSilo, _T( "model/mesh" ), _T( "msh" ) ),
	TMimeType( CMimeType::HttpMimeMshMeshSilo, _T( "model/mesh" ), _T( "mesh" ) ),
	TMimeType( CMimeType::HttpMimeMshMeshSilo, _T( "model/mesh" ), _T( "silo" ) ),
	TMimeType( CMimeType::HttpMimeWrlVrml, _T( "model/vrml" ), _T( "wrl" ) ),
	TMimeType( CMimeType::HttpMimeWrlVrml, _T( "model/vrml" ), _T( "vrml" ) ),
	TMimeType( CMimeType::HttpMimeCss, _T( "text/css" ), _T( "css" ) ),
	TMimeType( CMimeType::HttpMimeAscTxt, _T( "text/plain" ), _T( "asc" ) ),
	TMimeType( CMimeType::HttpMimeAscTxt, _T( "text/plain" ), _T( "txt" ) ),
	TMimeType( CMimeType::HttpMimeRtx, _T( "text/richtext" ), _T( "rtx" ) ),
	TMimeType( CMimeType::HttpMimeRtf, _T( "text/rtf" ), _T( "rtf" ) ),
	TMimeType( CMimeType::HttpMimeSgmlSgm, _T( "text/sgml" ), _T( "sgml" ) ),
	TMimeType( CMimeType::HttpMimeSgmlSgm, _T( "text/sgml" ), _T( "sgm" ) ),
	TMimeType( CMimeType::HttpMimeTsv, _T( "text/tab-separated-values" ), _T( "tsv" ) ),
	TMimeType( CMimeType::HttpMimeWml, _T( "text/vnd.wap.wml" ), _T( "wml" ) ),
	TMimeType( CMimeType::HttpMimeWmlc, _T( "text/vnd.wap.wmlscript" ), _T( "wmls" ) ),
	TMimeType( CMimeType::HttpMimeEtx, _T( "text/x-setext" ), _T( "etx" ) ),
	TMimeType( CMimeType::HttpMimeXslXml, _T( "text/xml" ), _T( "xsl" ) ),
	TMimeType( CMimeType::HttpMimeXslXml, _T( "text/xml" ), _T( "xml" ) ),

	// add by thinkingl !!!!
	TMimeType( CMimeType::HttpMimeXslXml, _T( "application/xml" ), _T( "xml" ) ),

	TMimeType( CMimeType::HttpMimeMpegMpgMpe, _T( "video/mpeg" ), _T( "mpeg" ) ),
	TMimeType( CMimeType::HttpMimeMpegMpgMpe, _T( "video/mpeg" ), _T( "mpg" ) )	,
	TMimeType( CMimeType::HttpMimeMpegMpgMpe, _T( "video/mpeg" ), _T( "mpe" ) ),
	TMimeType( CMimeType::HttpMimeQtMov, _T( "video/quicktime" ), _T( "qt" ) ),
	TMimeType( CMimeType::HttpMimeQtMov, _T( "video/quicktime" ), _T( "mov" ) ),
	TMimeType( CMimeType::HttpMimeMxu, _T( "video/vnd.mpegurl" ), _T( "mxu" ) ),
	TMimeType( CMimeType::HttpMimeAvi, _T( "video/x-msvideo" ), _T( "avi" ) ),
	TMimeType( CMimeType::HttpMimeMovie, _T( "video/x-sgi-movie" ), _T( "movie" ) ),
	TMimeType( CMimeType::HttpMimeIce, _T( "x-conference/x-cooltalk" ), _T( "ice" ) ),


	TMimeType( CMimeType::HttpMimeOther, _T(""), _T("") )

};

CMimeType::CMimeType(void)
{
	
}

CMimeType::~CMimeType(void)
{
}

BOOL CMimeType::Parse( LPCTSTR strUrl, LPCTSTR strContentType )
{
	this->m_strUrl = strUrl;
	this->m_strContentType = strContentType;

	tstringarray arFileExt;
	for ( int i=0; !s_arMimeTypeDic[i].m_strContentTypeToken.empty(); ++i )
	{
		if ( m_strContentType.find( s_arMimeTypeDic[i].m_strContentTypeToken ) != -1 )
		{
			this->m_eMimeType = s_arMimeTypeDic[i].m_eMimeType;
			arFileExt.push_back( s_arMimeTypeDic[i].m_strFileExt );
		}
	}

	tstring strUrlExt;
	int nPos = m_strUrl.rfind( _T( '.' ) );
	if ( nPos != -1 )
	{
		strUrlExt = this->m_strUrl.substr( nPos );
	}

	if ( arFileExt.empty() )
	{
//		ASSERT( FALSE );
		Log() << _T( "CMimeType::Parse Unknown content type! type: " ) << strContentType 
			<< _T( " url " ) << strUrl << endl;
		this->m_strRemommendExt = strUrlExt;
		return TRUE;
	}
	else
	{
		this->m_strRemommendExt = arFileExt[0];
		if ( !strUrlExt.empty() )
		{			
			for ( size_t i=0; i<arFileExt.size(); ++i )
			{
				if ( strUrlExt == arFileExt[i] )
				{
					m_strRemommendExt = arFileExt[i];
				}
			}
		}

//		ASSERT( FALSE );
		return TRUE;
	}
	
}

tstring CMimeType::GetRecommendFileExt() const
{
//	ASSERT( FALSE );
	return this->m_strRemommendExt;
}

CMimeType::EMimeType CMimeType::GetMimeType() const
{
//	ASSERT( FALSE );
	return this->m_eMimeType;
}