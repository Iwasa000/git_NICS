using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RectInfoCompareTool
{
    /// <summary>
    /// RectInfo比較クラス
    /// </summary>
    internal class RectInfoComparer
    {
        /// <summary>比較設定</summary>
        private RectInfoCompareSettings m_compareSettings;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="compareSettings">比較設定</param>
        public RectInfoComparer(RectInfoCompareSettings compareSettings)
        {
            m_compareSettings = compareSettings ?? throw new ArgumentNullException();
        }

        /// <summary>
        /// 各行の比較を行う
        /// </summary>
        /// <param name="rectInfos">RectInfo文字列のリスト</param>
        /// <returns></returns>
        public RectInfoCompareResult CompareLines(IList<string> rectInfos)
        {
            var result = new RectInfoCompareResult();
            var compValueFactory = new RectInfoLineCompareValueFactory(m_compareSettings);
            var rectInfoConverter = new RectInfoConverter();
            var cellLines = CreateCellLines(rectInfos);
            for (int i = 0; i < m_compareSettings.LineCellCount;  i++)
            {
                // 行の比較
                var cellLinesCompResult = CompareCellLines(compValueFactory, cellLines[i]);
                var errorMessages = new List<string>();
                if (cellLinesCompResult.DifferentValueCount > 1)
                {
                    errorMessages.Add($"{cellLinesCompResult.DifferentValueCount}個の異なる設定の行が存在します。");
                }

                // 枠線が消える設定の存在チェック
                if (HasBorderDisappearLine(cellLines[i]))
                {
                    errorMessages.Add($"枠線が消える可能性のある設定が存在します。");
                }

                if (errorMessages.Any())
                {
                    result.ErrorCellIndexes.Add(i);
                    result.ErrorMessages.Add(i, errorMessages);
                }

                result.CellSettingLines.Add(string.Join(Environment.NewLine, cellLines[i].Select(x => x.CreateLine())));
            }

            return result;
        }

        /// <summary>
        /// セルごとの行リストを生成する
        /// </summary>
        /// <param name="rectInfos">RectInfo文字列のリスト</param>
        /// <returns></returns>
        private IList<List<RectInfoLine>> CreateCellLines(IList<string> rectInfos)
        {
            var converter = new RectInfoConverter();
            var rectInfoLines = converter.Parse(rectInfos);

            var cellLines = new List<RectInfoLine>[m_compareSettings.LineCellCount];
            for (int i = 0; i < m_compareSettings.LineCellCount; i++)
            {
                cellLines[i] = new List<RectInfoLine>();
            }

            int cellI = 0;
            foreach (var rectInfoLine in rectInfoLines)
            {
                cellLines[cellI++].Add(rectInfoLine);
                if (cellI == m_compareSettings.LineCellCount)
                {
                    cellI = 0;
                }
            }

            return cellLines;
        }

        /// <summary>
        /// セルごとの各行を比較する
        /// </summary>
        /// <param name="compValueFactory"></param>
        /// <param name="cellLines"></param>
        /// <returns></returns>
        private CellLinesCompareResult CompareCellLines(RectInfoLineCompareValueFactory compValueFactory, IEnumerable<RectInfoLine> cellLines)
        {
            var compareValueHash = new HashSet<string>();
            foreach (var cellLine in cellLines)
            {
                compareValueHash.Add(compValueFactory.CreateCompareValue(cellLine));
            }

            var result = new CellLinesCompareResult();
            result.DifferentValueCount = compareValueHash.Count;
            return result;
        }

        /// <summary>
        /// セルごとの各行を比較した結果
        /// </summary>
        private class CellLinesCompareResult
        {
            /// <summary>含まれる比較値の数</summary>
            public int DifferentValueCount { get; set; } = 0;
        }

        /// <summary>
        /// 枠線が消える設定の行が存在するか判定する
        /// </summary>
        /// <param name="cellLines"></param>
        /// <returns></returns>
        private bool HasBorderDisappearLine(IEnumerable<RectInfoLine> cellLines)
        {
            if (!m_compareSettings.IsCheckBorderDisappears)
            {
                return false;
            }

            foreach (var line in cellLines)
            {
                if (line.Height == 7 && line.FontSize > 90)
                {
                    return true;
                }
            }

            return false;
        }
    }
}
