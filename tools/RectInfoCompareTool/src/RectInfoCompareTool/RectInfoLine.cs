using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RectInfoCompareTool
{
    /// <summary>
    /// RectInfoの行
    /// </summary>
    internal class RectInfoLine
    {
        private enum Columns
        {
            LineType = 0,
            PosX = 1,
            PosY = 2,
            Width = 3,
            Height = 4,
        }

        /// <summary>行をカンマで区切った要素のリスト</summary>
        private IList<string> m_lineElements;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="line">RectInfoの行テキスト</param>
        public RectInfoLine(string line)
        {
            // カンマ区切りだが、カンマの後にSOHがある場合は区切りとして扱わないようにする
            m_lineElements = new List<string>();
            var buf = new StringBuilder();
            for (int i = 0; i < line.Length; i++)
            {
                char c = line[i];
                if (c == ',')
                {
                    bool nextIsSOH = (i + 1 < line.Length && line[i + 1] == '\x01');
                    if (nextIsSOH)
                    {
                        buf.Append(c);
                    }
                    else
                    {
                        m_lineElements.Add(buf.ToString());
                        buf.Clear();
                    }
                }
                else
                {
                    buf.Append(c);
                }
            }

            m_lineElements.Add(buf.ToString());
        }

        /// <summary>
        /// RectInfoの行を生成する
        /// </summary>
        /// <returns></returns>
        public string CreateLine()
        {
            return string.Join(",", m_lineElements);
        }

        /// <summary>行タイプ</summary>
        public string LIneType => m_lineElements[(int)Columns.LineType];

        /// <summary>X座標</summary>
        public int PosX
        {
            get => int.Parse(m_lineElements[(int)Columns.PosX]);
            set => m_lineElements[(int)Columns.PosX] = value.ToString();
        }

        /// <summary>Y座標</summary>
        public int PosY
        {
            get => int.Parse(m_lineElements[(int)Columns.PosY]);
            set => m_lineElements[(int)Columns.PosY] = value.ToString();
        }

        /// <summary>幅</summary>
        public int Width
        {
            get
            {
                if (this.LIneType == "1")
                {
                    return 0;
                }
                else
                {
                    return int.Parse(m_lineElements[(int)Columns.Width]);
                }
            }
            set
            {
                if (this.LIneType != "1")
                {
                    m_lineElements[(int)Columns.Width] = value.ToString();
                }
            }
        }

        /// <summary>高さ</summary>
        public int Height
        {
            get
            {
                if (this.LIneType == "1")
                {
                    return 0;
                }
                else
                {
                    return int.Parse(m_lineElements[(int)Columns.Height]);
                }
            }
            set
            {
                if (this.LIneType != "1")
                {
                    m_lineElements[(int)Columns.Height] = value.ToString();
                }
            }
        }

        /// <summary>フォント名</summary>
        public string FontName
        {
            get
            {
                var col = GetFontNameColumn();
                return col >= 0 ? m_lineElements[col] : "";
            }
            set
            {
                var col = GetFontNameColumn();
                if (col >= 0)
                {
                    m_lineElements[col] = value;
                }
            }
        }

        /// <summary>フォント名列を取得する</summary>
        private int GetFontNameColumn()
        {
            switch (this.LIneType)
            {
                case "0": return 25;
                case "3": return 21;
                default:
                    return -1;
            }
        }

        /// <summary>フォントサイズ</summary>
        public int FontSize
        {
            get
            {
                var col = GetFontSizeColumn();
                return col >= 0 ? int.Parse(m_lineElements[col]) : -1;
            }
            set
            {
                var col = GetFontSizeColumn();
                if (col >= 0)
                {
                    m_lineElements[col] = value.ToString();
                }
            }
        }

        /// <summary>フォントサイズ列を取得する</summary>
        private int GetFontSizeColumn()
        {
            switch (this.LIneType)
            {
                case "0": return 28;
                case "3": return 35;
                default:
                    return -1;
            }
        }
    }
}
