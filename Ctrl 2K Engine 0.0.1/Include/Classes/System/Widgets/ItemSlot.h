#pragma once
/*

Item Slot object used for placing items on

*/

class ItemSlot : public Widget
{
public:
	ItemSlot();
	virtual ~ItemSlot();

private:
	int m_currentItem; //itemIndex

public:
	void drawWidget(); //draws widget onto screen
};

/*

end item slot object

*/