unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  ExtCtrls, wizzard;

type

  { TfrmMain }

  TfrmMain = class(TForm)
    btInstalar: TButton;
    btDocumentacao: TButton;
    btSite: TButton;
    Image1: TImage;
    Shape1: TShape;
    procedure btInstalarClick(Sender: TObject);
    procedure Shape1ChangeBounds(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.lfm}

{ TfrmMain }

procedure TfrmMain.Shape1ChangeBounds(Sender: TObject);
begin

end;

procedure TfrmMain.btInstalarClick(Sender: TObject);
begin
  frmWizzard := TfrmWizzard.create(self);
  frmWizzard.showmodal();
  frmWizzard.Free;
  frmWizzard := nil;
end;

end.

