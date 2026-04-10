namespace RectInfoCompareTool
{
    partial class RectInfoInput
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
            this.PasteButton = new System.Windows.Forms.Button();
            this.RectInfoText = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // TitleLabel
            // 
            this.TitleLabel.Location = new System.Drawing.Point(0, 0);
            this.TitleLabel.Name = "TitleLabel";
            this.TitleLabel.Size = new System.Drawing.Size(82, 24);
            this.TitleLabel.TabIndex = 0;
            this.TitleLabel.Text = "RectInfo9";
            this.TitleLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // PasteButton
            // 
            this.PasteButton.Location = new System.Drawing.Point(88, 0);
            this.PasteButton.Name = "PasteButton";
            this.PasteButton.Size = new System.Drawing.Size(82, 24);
            this.PasteButton.TabIndex = 1;
            this.PasteButton.Text = "貼り付け";
            this.PasteButton.UseVisualStyleBackColor = true;
            this.PasteButton.Click += new System.EventHandler(this.PasteButton_Click);
            // 
            // RectInfoText
            // 
            this.RectInfoText.AcceptsReturn = true;
            this.RectInfoText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RectInfoText.Location = new System.Drawing.Point(176, 0);
            this.RectInfoText.Multiline = true;
            this.RectInfoText.Name = "RectInfoText";
            this.RectInfoText.Size = new System.Drawing.Size(324, 24);
            this.RectInfoText.TabIndex = 2;
            this.RectInfoText.WordWrap = false;
            // 
            // RectInfoInput
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.RectInfoText);
            this.Controls.Add(this.PasteButton);
            this.Controls.Add(this.TitleLabel);
            this.Font = new System.Drawing.Font("BIZ UDゴシック", 12F);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "RectInfoInput";
            this.Size = new System.Drawing.Size(500, 24);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label TitleLabel;
        private System.Windows.Forms.Button PasteButton;
        private System.Windows.Forms.TextBox RectInfoText;
    }
}
