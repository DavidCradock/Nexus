#pragma once
#include "../precompiled_header.h"
#include "../math/vector2.h"

namespace Nexus
{
	// A GUI object, a line graph.
	class GUILineGraph
	{
		friend class GUIWindow;
	public:
		// Sets default settings for the object
		GUILineGraph();

		// Called from GUIWindow to render this object
		void render(GUIWindow* pWindow);

		// Sets position within it's parent window
		void setPosition(const Vector2& vNewPosition) { vPosition = vNewPosition; }

		// Sets position within it's parent window
		void setPosition(int iPosX, int iPosY) { vPosition.set((float)iPosX, (float)iPosY); }

		// Sets dimensions of the object
		void setDimenions(int iNewWidth, int iNewHeight) { vDimensions.x = (float)iNewWidth; vDimensions.y = (float)iNewHeight; }

		// Sets the text to be rendered for the X axis
		void setTextX(const std::string& text) { strTextX = text; }

		// Sets the text to be rendered for the X axis
		void setTextY(const std::string& text) { strTextY = text; }

		// Adds a new value to the linegraph
		void addValue(float fValue);

		// Sets maximum number of values
		void setMaxNumValues(int iMaxNumValues);

		// Removes all values
		void removeAllValues(void);
	private:
		Vector2 vDimensions;	// Width and height of the object
		Vector2 vPosition;		// Position of this object relative to it's parent window.
		std::string strTextX;	// Text rendered for the X axis
		std::string strTextY;	// Text rendered for the Y axis
		std::list<float> listValues;	// Values for Y axis
		int iMaxNumberValues;			// Maximum number of values to keep
		float fMaxValue;				// Holds the highest value added to listValues
		float fMinValue;				// Holds the lowest value added to listValues
	};

}
