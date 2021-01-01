unit Wizzard;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  StdCtrls;

type

  { TfrmWizzard }

  TfrmWizzard = class(TForm)
    btSair: TButton;
    btSeguir: TButton;
    Image1: TImage;
    Label1: TLabel;
    procedure btSairClick(Sender: TObject);
    procedure btSeguirClick(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  frmWizzard: TfrmWizzard;

implementation

{$R *.lfm}

{ TfrmWizzard }

procedure TfrmWizzard.btSairClick(Sender: TObject);
begin
  close;
end;

procedure TfrmWizzard.btSeguirClick(Sender: TObject);
begin
  //Incluir o controle do wizzard

  close;
end;

end.

