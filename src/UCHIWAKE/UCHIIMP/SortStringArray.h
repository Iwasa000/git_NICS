class CSortStringArray : public CStringArray {
public:
	void Sort();
private:
	BOOL CompareAndSwap(int pos);
};
