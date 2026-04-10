using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RectInfoCompareTool
{
    /// <summary>
    /// RectInfo変換クラス
    /// </summary>
    internal class RectInfoConverter
    {
        /// <summary>
        /// RectInfoをパースする
        /// </summary>
        /// <param name="rectInfo"></param>
        /// <returns></returns>
        public IEnumerable<RectInfoLine> Parse(string rectInfo)
        {
            rectInfo = rectInfo ?? string.Empty;
            var lines = rectInfo.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
            return lines.Select(x => new RectInfoLine(x));
        }

        /// <summary>
        /// RectInfoをパースする
        /// </summary>
        /// <param name="rectInfos"></param>
        /// <returns></returns>
        public List<RectInfoLine> Parse(IList<string> rectInfos)
        {
            var rectInfoLines = new List<RectInfoLine>();
            foreach (var ri in rectInfos)
            {
                rectInfoLines.AddRange(Parse(ri));
            }

            return rectInfoLines;
        }

        /// <summary>
        /// RectInfo文字列を生成する
        /// </summary>
        /// <param name="rectInfos"></param>
        /// <returns></returns>
        public string CreateRectInfoText(IEnumerable<RectInfoLine> rectInfos)
        {
            return string.Join("\\", rectInfos.Select(x => x.CreateLine()));
        }
    }
}
