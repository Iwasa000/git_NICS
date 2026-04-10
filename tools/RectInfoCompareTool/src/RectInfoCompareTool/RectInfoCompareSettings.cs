using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RectInfoCompareTool
{
    /// <summary>
    /// 比較設定
    /// </summary>
    internal class RectInfoCompareSettings
    {
        public int LineCellCount;
        public bool IsCheckHeight;
        public bool IsCheckWidth;
        public bool IsCheckFontName;
        public bool IsCheckFontSize;

        /// <summary>枠線が消える設定をチェックするか</summary>
        public bool IsCheckBorderDisappears;
    }
}
