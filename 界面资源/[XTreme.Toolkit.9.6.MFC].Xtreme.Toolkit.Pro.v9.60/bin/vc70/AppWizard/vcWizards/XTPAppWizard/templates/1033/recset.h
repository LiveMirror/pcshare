// [!output ROWSET_HEADER]: interface of the [!output ROWSET_CLASS] class
//


#pragma once

[!if ODBC]
[!output ROWSET_CLASS_ODBC_DECL]
[!else]
[!output ROWSET_CLASS_CODE]
[!endif]
