using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RectInfoCompareTool
{
    /// <summary>
    /// 比較結果
    /// </summary>
    internal class RectInfoCompareResult
    {
        /// <summary>各セルの設定テキストのリスト(セルインデックス順)</summary>
        public List<string> CellSettingLines { get; } = new List<string>();

        /// <summary>比較項目が一致しない行があるセルインデックスのリスト</summary>
        public List<int> ErrorCellIndexes { get; } = new List<int>();

        /// <summary>エラーがあるか</summary>
        public bool HasError => this.ErrorCellIndexes.Count > 0;

        /// <summary>セルインデックス -> エラーメッセージ</summary>
        public Dictionary<int, IList<string>> ErrorMessages { get; } = new Dictionary<int, IList<string>>();
    }
}
