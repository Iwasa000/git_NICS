class CErrBlk
{
public:
	CErrBlk( short code, char *msg = 0)
	{
		ErrCode = code;
		ErrMsg = msg ? msg : "";
	}

	~CErrBlk() {}

	short	ErrCode;
	CString ErrMsg;
};
