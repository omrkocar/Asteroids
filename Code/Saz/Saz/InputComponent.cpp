#include "SazPCH.h"
#include "InputComponent.h"

/*// Input-related functions: keyboard
RLAPI bool IsKeyPressed(int key);                             // Check if a key has been pressed once
RLAPI bool IsKeyDown(int key);                                // Check if a key is being pressed
RLAPI bool IsKeyReleased(int key);                            // Check if a key has been released once
RLAPI bool IsKeyUp(int key);                                  // Check if a key is NOT being pressed
RLAPI void SetExitKey(int key);                               // Set a custom key to exit program (default is ESC)
RLAPI int GetKeyPressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
RLAPI int GetCharPressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
* 
*/ 

//bool component::InputComponent::IsKeyHeld(Input::EKeyboard key) const
//{
//	return m_KeyboardCurrent.count(key) > 0;
//}
//
//bool component::InputComponent::IsKeyPressed(Input::EKeyboard key) const
//{
//	return m_KeyboardPrevious.count(key) == 0
//		&& m_KeyboardCurrent.count(key) > 0;
//}
//
//bool component::InputComponent::IsKeyReleased(Input::EKeyboard key) const
//{
//	return m_KeyboardPrevious.count(key) > 0
//		&& m_KeyboardCurrent.count(key) == 0;
//}
//
//bool component::InputComponent::IsKeyHeld(Input::EMouse key) const
//{
//	return m_MouseCurrent.count(key) > 0;
//}
//
//bool component::InputComponent::IsKeyPressed(Input::EMouse key) const
//{
//	return m_MousePrevious.count(key) == 0
//		&& m_MouseCurrent.count(key) > 0;
//}
//
//bool component::InputComponent::IsKeyReleased(Input::EMouse key) const
//{
//	return m_MousePrevious.count(key) > 0
//		&& m_MouseCurrent.count(key) == 0;
//}