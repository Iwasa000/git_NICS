namespace RectInfoCompareTool
{
    partial class CellLinesView
    {
        /// <summary> 
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region コンポーネント デザイナーで生成されたコード

        /// <summary> 
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.TitleLabel = new System.Windows.Forms.Label();
            this.LinesText = new System.Windows.Forms.TextBox();
            this.CopyButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // TitleLabel
            // 
            this.TitleLabel.Dock = System.Windows.Forms.DockStyle.Left;
            this.TitleLabel.Location = new System.Drawing.Point(0, 0);
            this.TitleLabel.Name = "TitleLabel";
            this.TitleLabel.Size = new System.Drawing.Size(58, 24);
            this.TitleLabel.TabIndex = 0;
            this.TitleLabel.Text = "セル99";
            this.TitleLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // LinesText
            // 
            this.LinesText.AcceptsReturn = true;
            this.LinesText.BackColor = System.Drawing.Color.White;
            this.LinesText.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LinesText.Location = new System.Drawing.Point(140, 0);
            this.LinesText.Multiline = true;
            this.LinesText.Name = "LinesText";
            this.LinesText.ReadOnly = true;
            this.LinesText.Size = new System.Drawing.Size(390, 24);
            this.LinesText.TabIndex = 2;
            this.LinesText.WordWrap = false;
            // 
            // CopyButton
            // 
            this.CopyButton.Dock = System.Windows.Forms.DockStyle.Left;
            this.CopyButton.Location = new System.Drawing.Point(58, 0);
            this.CopyButton.Name = "CopyButton";
            this.CopyButton.Size = new System.Drawing.Size(82, 24);
            this.CopyButton.TabIndex = 1;
            this.CopyButton.Text = "コピー";
            this.CopyButton.UseVisualStyleBackColor = true;
            this.CopyButton.Click += new System.EventHandler(this.CopyButton_Click);
            // 
            // CellLinesView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.LinesText);
            this.Controls.Add(this.CopyButton);
            this.Controls.Add(this.TitleLabel);
            this.Font = new System.Drawing.Font("BIZ UDゴシック", 12F);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "CellLinesView";
            this.Size = new System.Drawing.Size(530, 24);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label TitleLabel;
        private System.Windows.Forms.TextBox LinesText;
        private System.Windows.Forms.Button CopyButton;
    }
}
