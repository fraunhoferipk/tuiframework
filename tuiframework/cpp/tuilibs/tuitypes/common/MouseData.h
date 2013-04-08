#ifndef _MouseData_h_
#define _MouseData_h_

#include <tuiframework/core/ISerializable.h>

class MouseData : public tuiframework::ISerializable {
public:
	enum CODE {
			// Left Button
		BUTTON_1_DOWN = 0x0001,
		BUTTON_1_UP = 0x0002,
			// Right Button
		BUTTON_2_DOWN = 0x0004,
		BUTTON_2_UP = 0x0008,
			// Middle Button
		BUTTON_3_DOWN = 0x0010,
		BUTTON_3_UP = 0x0020,

		BUTTON_4_DOWN = 0x0040,
		BUTTON_4_UP = 0x0080,
		BUTTON_5_DOWN = 0x100,
		BUTTON_5_UP = 0x0200
	};
	MouseData();
	MouseData(int id);
	virtual ~MouseData();

	void setId(int id);
	void setDeltaX(int deltaX);
	void setDeltaY(int deltaY);
	void setDeltaWheel(int deltaWheel);
	void setButtonCode(unsigned int buttonCode);

	int getId() const;
	int getDeltaX() const;
	int getDeltaY() const;
	int getDeltaWheel() const;
	bool getButtonStateChange(CODE code) const;

	bool hasChanged() const;

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
	int id;
	int deltaX;
	int deltaY;
	int deltaWheel;
	unsigned int buttonCode;
};

#endif