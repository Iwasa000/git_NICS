using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RectInfoCompareTool
{
    /// <summary>
    /// RectInfo行の比較値ファクトリー
    /// </summary>
    internal class RectInfoLineCompareValueFactory
    {
        /// <summary>
        /// 比較値取得処理
        /// </summary>
        /// <param name="line"></param>
        /// <returns></returns>
        private delegate string CompareValueGetFunc(RectInfoLine line);

        /// <summary>比較値取得処理リスト</summary>
        private List<CompareValueGetFunc> m_compareValueGetters = new List<CompareValueGetFunc>();

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="compareSettings">比較設定</param>
        public RectInfoLineCompareValueFactory(RectInfoCompareSettings compareSettings)
        {
            if (compareSettings == null)
            {
                throw new ArgumentNullException();
            }

            if (compareSettings.IsCheckHeight)
            {
                m_compareValueGetters.Add((line) => line.Height.ToString());
            }

            if (compareSettings.IsCheckWidth)
            {
                m_compareValueGetters.Add((line) => line.Width.ToString());
            }

            if (compareSettings.IsCheckFontName)
            {
                m_compareValueGetters.Add((line) => line.FontName.ToString());
            }

            if (compareSettings.IsCheckFontSize)
            {
                m_compareValueGetters.Add((line) => line.FontSize.ToString());
            }
        }

        /// <summary>
        /// 行比較値を生成する
        /// </summary>
        /// <param name="line"></param>
        /// <returns></returns>
        public string CreateCompareValue(RectInfoLine line)
        {
            var valueBuilder = new StringBuilder();
            foreach (var compareValueGetter in m_compareValueGetters)
            {
                valueBuilder.Append(compareValueGetter.Invoke(line));
                valueBuilder.Append("$");
            }

            return valueBuilder.ToString();
        }
    }
}
