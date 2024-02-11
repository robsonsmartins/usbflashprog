//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("uEprom.cpp", frmEprom);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->Title = "Willem Eprom ";
        Application->CreateForm(__classid(TfrmEprom), &frmEprom);
     Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}

//---------------------------------------------------------------------------
/*   Height,width,left,top
  line 2721
  frmEprom 440 490
  pProcess 97,386,48,240
  pIDcode  120,240,112,225
  pdLockBit 105,220,120,135
*/
