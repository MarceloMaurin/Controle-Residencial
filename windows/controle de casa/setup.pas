unit setup;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  Buttons;

type

  { TfrmSetup }

  TfrmSetup = class(TForm)
    Edit1: TEdit;
    edHost: TEdit;
    edUser: TEdit;
    edPassword: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  frmSetup: TfrmSetup;

implementation

{$R *.lfm}

end.

