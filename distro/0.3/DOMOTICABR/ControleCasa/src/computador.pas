unit computador;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, Buttons,
  StdCtrls;

type

  { TfrmComputador }

  TfrmComputador = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    lbIP: TLabel;
    lbComputador: TLabel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    procedure SpeedButton1Click(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  frmComputador: TfrmComputador;

implementation

{$R *.lfm}

{ TfrmComputador }

procedure TfrmComputador.SpeedButton1Click(Sender: TObject);
begin
  Close;
end;

end.

