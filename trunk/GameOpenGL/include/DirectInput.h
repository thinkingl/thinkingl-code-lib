#ifndef __INPUT_SYSTEM_INCLUDED__
#define __INPUT_SYSTEM_INCLUDED__

#include <dinput.h>


#define IS_USEKEYBOARD  1
#define IS_USEMOUSE     2
#define IS_USEJOYSTICK  4

class CKeyboard
{
public:
  CKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd);
  ~CKeyboard();

  bool  IsKeyDown(int key) { return (m_keys[key] & 0x80) ? true : false; }
  bool  IsKeyUp(int key) { return (m_keys[key] & 0x80) ? false : true; }

  bool  Update();

  void  Clear() { ZeroMemory(m_keys, 256 * sizeof(char)); }

  bool  Acquire();
  bool  Unacquire();

private:
  LPDIRECTINPUTDEVICE8  m_pDIDev;

  char    m_keys[256];
};


class CMouse
{
public:
  CMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive = true);
  ~CMouse();

  bool  IsButtonDown(int button) { return (m_state.rgbButtons[button] & 0x80) ? true : false; }
  bool  IsButtonUp(int button) { return (m_state.rgbButtons[button] & 0x80) ? false : true; }
  
  int GetMouseRelativeMoveX(){return m_state.lX;}
  int GetMouseRelativeMoveY(){return m_state.lY;}
  int GetMouseRelativeMoveZ(){return m_state.lZ;}

  bool  Update();

  bool  Acquire();
  bool  Unacquire();

private:
  LPDIRECTINPUTDEVICE8  m_pDIDev;
  DIMOUSESTATE          m_state;
};


class CJoystick
{
public:
  CJoystick(LPDIRECTINPUT8 pDI, HINSTANCE appInstance);
  ~CJoystick();

  bool  Update();

  bool  Acquire();
  bool  Unacquire();

private:
  LPDIRECTINPUTDEVICE8  m_pDIDev;
};


class DirectInput
{
public:
	DirectInput() { }
	~DirectInput() { Shutdown(); }
  bool  Initialize(HWND hwnd, HINSTANCE appInstance, bool isExclusive, DWORD flags = 0);
  bool  Shutdown();

  void  AcquireAll();
  void  UnacquireAll();

  CKeyboard * GetKeyboard() { return m_pKeyboard; }
  CMouse    * GetMouse()    { return m_pMouse; }
  CJoystick * GetJoystick() { return m_pJoystick; }

  bool  Update();

  bool  IsKeyDown(int key) { return (m_pKeyboard && m_pKeyboard->IsKeyDown(key)); }
  bool  IsKeyUp(int key) { return (m_pKeyboard && m_pKeyboard->IsKeyUp(key)); }

  bool  IsButtonDown(int button) { return (m_pMouse && m_pMouse->IsButtonDown(button)); }
  bool  IsButtonUp(int button) { return (m_pMouse && m_pMouse->IsButtonUp(button)); }

//  void  GetMouseMovement(int &dx, int &dy) { if (m_pMouse) m_pMouse->GetMovement(dx, dy); }

  int GetMouseRelativeMoveX(){ return (m_pMouse->GetMouseRelativeMoveX());}
  int GetMouseRelativeMoveY(){ return (m_pMouse->GetMouseRelativeMoveY());}
  int GetMouseRelativeMoveZ(){ return (m_pMouse->GetMouseRelativeMoveZ());}
private:
  CKeyboard  *m_pKeyboard;
  CMouse     *m_pMouse;
  CJoystick  *m_pJoystick;
  
  LPDIRECTINPUT8 m_pDI;
};


#endif //__INPUT_SYSTEM_INCLUDED__