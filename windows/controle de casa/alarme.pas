unit Alarme;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  ExtCtrls,dos;

type

  { TfrmAlarme }

  TfrmAlarme = class(TForm)
    Label2: TLabel;
    Timer1: TTimer;
    Timer2: TTimer;
    ToggleBox1: TToggleBox;
    procedure FormClose(Sender: TObject; var CloseAction: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Timer2Timer(Sender: TObject);
    procedure ToggleBox1Change(Sender: TObject);

  private
    { private declarations }
    //Mplayer2.exe /play /close C:\Asfroot\Sample.asf

  public
    { public declarations }

  end;

var
  frmAlarme: TfrmAlarme;

implementation

{$R *.lfm}

{ TfrmAlarme }
uses Controle;

procedure TfrmAlarme.FormCreate(Sender: TObject);
begin
  Timer1.Enabled:=true;
  Timer2.Enabled:=true;
  frmControle.falar.falar('Atenção voçe programaou um alarme.');

end;

procedure TfrmAlarme.FormClose(Sender: TObject; var CloseAction: TCloseAction);
begin
    Timer1.Enabled:=false;
  Timer2.Enabled:=false;
end;

procedure TfrmAlarme.Timer1Timer(Sender: TObject);
var
  Hora , Minuto, Seg, HSeg : word;
begin
   GETTime( Hora, minuto,seg,HSeg);
   frmControle.falar.falar('Agora são '+inttostr(Hora)+' horas e '+ inttostr(Minuto)+' minutos ');
end;

procedure TfrmAlarme.Timer2Timer(Sender: TObject);
begin
  label2.Caption:= timetostr(now);
end;

procedure TfrmAlarme.ToggleBox1Change(Sender: TObject);
begin

  close;
end;

end.
