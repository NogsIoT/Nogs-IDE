/*
 *
 *
 *  Copyright (C) Nogs GmbH, Andre Riesberg
 *  
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, 
 *  write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "Usul.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "JSON.h"
#include "MainFrm.h"
#include <System.StrUtils.hpp>
#include "Utils.h"
#include "UDP.h"
#include "COAP.h"
#include <Vcl.Clipbrd.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
static TColor Colors[] = {RGB(255, 255, 255), clSkyBlue, clMoneyGreen, clYellow, (TColor) RGB(255, 128, 0)};
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
AnsiString __fastcall GetPath(TTreeNode *TreeNode)
{
  AnsiString S;
  while (TreeNode->Level >= 1) {
    //if (S.Length())
    //  S = "." + S;
    S = ReplaceStr(TreeNode->Text, "*", "") + S;
    TreeNode = TreeNode->Parent;
  }
  return S;
}
//---------------------------------------------------------------------------
AnsiString __fastcall NBSP(const AnsiString S)
{
  return ReplaceStr(S, " ", "&nbsp;");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TreeItemToHTML(TTreeNode *TreeNode, int &Id)
{
  AnsiString Indent = AnsiString::StringOfChar(32, TreeNode->Level * 2 + 2);

  int RGB = ColorToRGB(Colors[(int) TreeNode->Data]);

  #if 1 // Darker
    RGB = (RGB >> 2) & 0x3F3F3F;
  #endif
  AnsiString Color = "#" + AnsiString::IntToHex(RGB & 0xFF, 2) + AnsiString::IntToHex((RGB >> 8) & 0xFF, 2) + AnsiString::IntToHex((RGB >> 16) & 0xFF, 2);


  AnsiString S;
  //if (TreeNode->ImageIndex == TYPE_PARAMETER) {
  if ((TFrameBrowser::TType) TreeNode->Data == TFrameBrowser::TYPE_PARAMETER) {

    for (int I = 0; I < TreeNode->Count; I++)
      S += Indent + "  <li  class=\"file\">" + TreeNode->Item[I]->Text + "</li>\n";

  } else {

    if (TreeNode->Text.SubString(1, 2) != "__") {
      AnsiString Link = GetPath(TreeNode);
      int P = Link.Pos("(");
      if (P)
        Link.SetLength(P - 1);
      P = Link.Pos(" = ");
      if (P)
        Link.SetLength(P - 1);
      if (Link.Length() && (Link[Link.Length()] == '-'))
        Link.SetLength(Link.Length() - 1);
      Link = "http://en.wiki.nogs.cc/index.php/" + Link;

      S += Indent + "<li>\n";
      if (TreeNode->Level >= 222)
        S += Indent + "  <label for=\"folder" + AnsiString(Id) + "\"><a href=\"" + Link + "\">" + NBSP(TreeNode->Text) + "</a></label><input type=\"checkbox\"/>\n";
      else {
        S += Indent + "  <label for=\"folder" + AnsiString(Id) + "\"><a href=\"" + Link + "\">" + NBSP(TreeNode->Text) + "</a></label><input type=\"checkbox\"/ checked >\n";
      }
      S += Indent + "  <ol>\n";

      for (int I = 0; I < TreeNode->Count; I++) {
        //S += Indent + "  <li>\n";
        Id++;
        S += "  " + TreeItemToHTML(TreeNode->Item[I], Id);
        //S += Indent + "  </li>\n";
      }
      S += Indent + "  </ol>\n";
      S += Indent + "</li>\n";
    }
  }

  return S;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TreeItemToWiki(TTreeNode *TreeNode)
{
  AnsiString Indent = AnsiString::StringOfChar('*', TreeNode->Level + 1);
  AnsiString S;
  if ((TFrameBrowser::TType) TreeNode->ImageIndex == TFrameBrowser::TYPE_PARAMETER) {

  } else {
    if (TreeNode->Level)
      S += "[[function:" + GetPath(TreeNode) + "|" + TreeNode->Text + "]]\n";
    for (int I = 0; I < TreeNode->Count; I++) {
      S += Indent + TreeItemToWiki(TreeNode->Item[I]);
    }
  }
  return S;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TreeItemToWikiLinks(TTreeNode *TreeNode)
{
  AnsiString Indent = AnsiString::StringOfChar('*', TreeNode->Level + 1);
  AnsiString S;
  if ((TFrameBrowser::TType) TreeNode->ImageIndex == TFrameBrowser::TYPE_PARAMETER) {
  } else {
    if (TreeNode->Level) {
      if (TreeNode->Text.SubString(1, 2) != "__") {
        AnsiString Link = GetPath(TreeNode);
        int P = Link.Pos("(");
        if (P)
          Link.SetLength(P - 1);
        P = Link.Pos(" = ");
        if (P)
          Link.SetLength(P - 1);
        if (Link.Length() && (Link[Link.Length()] == '-'))
          Link.SetLength(Link.Length() - 1);

        S += "[[" + Link + "|" + TreeNode->Text + "]]\n";
      }
    }
    for (int I = 0; I < TreeNode->Count; I++) {
      AnsiString S2 = TreeItemToWikiLinks(TreeNode->Item[I]);
      if (S2.Length())
        S += Indent + S2;
    }
  }
  return S;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
TTreeNode* TreeNodeSet(TTreeNode *TreeNode, int Data, int ImageIndex)
{
  TreeNode->Data = (void*) Data;
  TreeNode->ImageIndex = ImageIndex;
  TreeNode->SelectedIndex = ImageIndex;
  return TreeNode;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::TreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *TreeNode, TCustomDrawState State, bool &DefaultDraw)
{
  TCanvas *C = TreeView->Canvas;
  C->Brush->Color = (TColor) RGB(25, 25, 25);
  C->Font->Color = Colors[(int) TreeNode->Data];
  if (TreeNode->ImageIndex && (TreeNode->ImageIndex < TYPE_VALUE))
    C->Font->Style = C->Font->Style << fsUnderline;
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::TreeViewGetSelectedIndex(TObject *Sender, TTreeNode *TreeNode)
{
  TreeNode->SelectedIndex = TreeNode->ImageIndex;
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::TreeViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  MouseX = X;
  MouseY = Y;
  THitTests HitTests = TreeView->GetHitTestInfoAt(X, Y);
  if (HitTests.Contains(htOnLabel)) {
    TTreeNode *TreeNode = TreeView->Selected;
    if ((int) TreeNode->Data == TYPE_PARAMETER) {
//      String Function = ReplaceStr(TreeNode->Parent->Text, "()", EmptyStr);
//      String Library = ReplaceStr(TreeNode->Parent->Parent->Text, ".*", EmptyStr);
//      ShellExecute(0, L"open", ("http://en.wiki.nogs.cc/index.php/" + Library + "." + Function).c_str(), 0, 0, SW_SHOWNORMAL);
    } else if (!TreeNode->Expanded) 
      Expand(TreeNode);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::ButtonBrowseClick(TObject *Sender)
{
  TreeView->Items->BeginUpdate();
  TreeView->Items->Clear();
  TTreeNode *TreeNode = TreeView->Items->Add(0, IP.ToString());
  TreeNode->ImageIndex = Type;
  TreeNodeSet(TreeView->Items->AddChild(TreeNode, "sys.*"), TYPE_LIBRARY, -1);
  TreeNodeSet(TreeView->Items->AddChild(TreeNode, "hal.*"), TYPE_LIBRARY, -1);
  if (Type >= TTarget::CN)
    TreeNodeSet(TreeView->Items->AddChild(TreeNode, "app.*"), TYPE_LIBRARY, -1);
  TreeView->Items->EndUpdate();
  TreeNode->Expand(false);

  if (GetKeyState(VK_LSHIFT) & 0x8000) {
    ExpandTrigger = new TStringList;
    try {
      int Count;
      do {
        TreeView->Items->BeginUpdate();
        Count = TreeView->Items->Count;
        for (int I = 0; I < TreeView->Items->Count; I++) {
          TTreeNode *TreeNode = TreeView->Items->Item[I];
          String FullPath;
          {
            TTreeNode *TreeNode2 = TreeNode;
            while (TreeNode2->Level) { 
              FullPath += "/" + TreeNode2->Text;
              TreeNode2 = TreeNode2->Parent;
            }
          }
          if (ExpandTrigger->IndexOf(FullPath) < 0) {
            ExpandTrigger->Add(FullPath);
            Expand(TreeNode);
          }
          //Application->ProcessMessages();
        }
        for (int I = 0; I < 100; I++) {
          Application->ProcessMessages();
          Sleep(10);
        }
        TreeView->Items->EndUpdate();
     
      } while (Count != TreeView->Items->Count);
    } catch (bool) {
      TreeView->Items->EndUpdate();
    }
    delete ExpandTrigger, ExpandTrigger = 0;
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool __fastcall TFrameBrowser::Expand(TTreeNode *TreeNode)
{
  uint32_t TransactionId = (uint32_t) TreeNode;
  if ((int) TreeNode->Data == TYPE_LIBRARY) {

    String Library;
    while (TreeNode->Level >= 1) {
      if (Library.Length()) 
        Library = "." + Library;
      Library = ReplaceStr(TreeNode->Text, ".*", EmptyStr) + Library;
      TreeNode = TreeNode->Parent;
    }
    switch (Type) {

      case TTarget::PN :
        BrowsePN(Library, EmptyStr, TransactionId);
        break;

      case TTarget::CN :
        BrowseCN(Library, EmptyStr, TransactionId);
        break;

      default :
        return false;

    }

  } else if ((int) TreeNode->Data == TYPE_FUNCTION) {

    String Name = ReplaceStr(TreeNode->Text, "()", EmptyStr);
    String Library;
    TTreeNode *TreeNode2 = TreeNode->Parent;
    while (TreeNode2->Level >= 1) {
      if ((int) TreeNode2->Data != TYPE_LIBRARY) {
        int I = 0;
        throw false;
      }
      if (Library.Length()) 
        Library = "." + Library;
      Library = ReplaceStr(TreeNode2->Text, ".*", EmptyStr) + Library;
      TreeNode2 = TreeNode2->Parent;
    }
    switch (Type) {

      case TTarget::CN :
        BrowseCN(Library, Name, TransactionId);
        break;

      default :
        return false;

    }

  } else
    return false;

  return true;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameBrowser::TFrameBrowser(TComponent* Owner)
  : TFrame(Owner), ExpandTrigger(0)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::Init()
{
  TreeView->Images = FormMain->ImageListNodeTypes;
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::Finish()
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::BrowsePN(const String Library, const String Function, uint32_t TransactionsId)
{
  if (IP.ToString() == "127.0.0.1") {

    TUDP *UDP = FormMain->FrameTargets->UnitTestUDP;
    TTreeNode *TreeNode = FormMain->Log(0, "CON");
    UDP->Send(TCOAP::JSONWrap("[\"sys:introduce()\"]", true));
    AnsiString Recv = UDP->Recv(500);
    if (Recv.IsEmpty()) return;
    AnsiString RecvJSON = TCOAP::JSONUnwrap(Recv);
    FormMain->Log(TreeNode, 1, RecvJSON);
    RecvJSON = TCOAP::JSONUnwrap(Recv);
    try {
      TJSONArray JSONArray(RecvJSON);
    }
    catch (TJSONScanner::TError &Error) {}
    TreeNode->Expand(false);

  } else {

    TUDP UDP;
    if (!UDP.Connect(IP, TCOAP::Port, TUDP::Client)) return;
    TTreeNode *TreeNode = FormMain->Log(0, "CON");
    UDP.Send(TCOAP::JSONWrap("[\"sys:introduce()\"]", true));
    AnsiString Recv = UDP.Recv(500);
    if (Recv.IsEmpty()) return;
    AnsiString RecvJSON = TCOAP::JSONUnwrap(Recv);
    FormMain->Log(TreeNode, 1, RecvJSON);
    try {
      TJSONArray JSONArray(RecvJSON);
    }
    catch (TJSONScanner::TError &Error) {}
    TreeNode->Expand(false);

  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::BrowseCN(const String Library, const String Function, uint32_t TransactionsId)
{
  if (Function.Length())
    FormMain->Backdoor(IP, "return \"[[\\\"Browser(" + String::IntToHex(TransactionsId, 8) + ")\\\"],\".." + Library + ":introduce('" + Function + "')..']'");
  else
    FormMain->Backdoor(IP, "return \"[[\\\"Browser(" + String::IntToHex(TransactionsId, 8) + ")\\\"],\".." + Library + ":introduce()..']'");
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::BrowseSN(const String Library, const String Function, uint32_t TransactionsId)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::Set(netIP4 _IP, TTarget::TNodeType _Type)
{
  IP = _IP;
  Type = _Type;
  ButtonBrowseClick(0);
  ButtonBrowse->Enabled = Type > 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::Recv(TJSONArray *Header, TJSONArray *Response)
{
  String Owner = Header->GetName(0);
  if (Owner.SubString(1, 8) == "Browser(") {
    uint32_t TransactionsId = TUtils::XToI(Owner.SubString(9, 8));
    TTreeNode *TreeNode = TreeView->Items->Item[0];
    for (int I = 0; I < TreeView->Items->Count; I++)
      if (TreeView->Items->Item[I] == (TTreeNode*) TransactionsId) {
        TreeNode = (TTreeNode*) TransactionsId;
        break;
      }
    TreeNode->DeleteChildren();
    for (int I = 0; I < 1000; I++) {
      TJSONObject *Value = Response->GetJSONObject(I);
      if (Value) {
        TreeNodeSet(TreeView->Items->AddChild(TreeNode, Value->GetName(0) + " = " + Value->GetString(0)), TYPE_VALUE, -1);
      } else {
        String S = Response->GetName(I);
        if (S.IsEmpty()) 
          break;
        TTreeNode *TreeNode2 = TreeNodeSet(TreeView->Items->AddChild(TreeNode, S), 0, -1);
        if (S.Pos(".*") || S.Pos(":*"))
          TreeNode2->Data = (void*) TYPE_LIBRARY;
        else if (S.Pos("(") && (S.Pos(".") || S.Pos(":")) && S.Pos(")"))
          TreeNode2->Data = (void*) TYPE_PARAMETER;
        else if (S.Pos("()"))
          TreeNode2->Data = (void*) TYPE_FUNCTION;
      }
    }
    TreeNode->Expand(false);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::MenuItemCopyAsHTMLClick(TObject *Sender)
{
  TTreeNode *TreeNodeRoot = TreeView->Items->Item[0];
  TreeNodeRoot->Text = "Stamp CN1";
  int Id = 1;
  String S;
  S += "<ol class=\"tree\">\n";
  S += TreeItemToHTML(TreeNodeRoot, Id);
  S += "</ol>\n";
  String HTML = TUtils::LoadTextFile(Usul()->Path + "\\HTMLTreeView\\template.html");
  HTML = ReplaceStr(HTML, "%TREEVIEW%", S);
  TUtils::SaveTextFile(HTML, Usul()->Path + "\\HTMLTreeView\\index.html");
  Clipboard()->SetTextBuf(S.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::CopyforWiki1Click(TObject *Sender)
{
  String S;
  S += TreeItemToWiki(TreeView->Items->Item[0]);
  Clipboard()->SetTextBuf(S.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFrameBrowser::CopyforWikilinks1Click(TObject *Sender)
{
  String S;
  S += TreeItemToWikiLinks(TreeView->Items->Item[0]);
  Clipboard()->SetTextBuf(S.c_str());
}
//---------------------------------------------------------------------------









