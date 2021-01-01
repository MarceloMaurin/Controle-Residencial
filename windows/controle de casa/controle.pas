unit Controle;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  ExtCtrls, Buttons, main, despertador, multimidia, cameras, espeak, Alarme,
  dados;

type

  { TfrmControle }

  TfrmControle = class(TForm)
    Image1: TImage;
    ImageList1: TImageList;
    lbHoras: TLabel;
    lbDia: TLabel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    Timer1: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Image1DblClick(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure SpeedButton4Click(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure SpeedButton6Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private
    { private declarations }
    procedure checaAlarme();
    procedure checaFotos();
    procedure TrocaFoto();
    procedure CarregaFotos();
    procedure ResetClock();
    //Chama Alarme
    procedure ChamaAlarme();
  public
    { public declarations }
    tempo : extended;
    Fotos : TStringList;
    diretorio: String;
    diretorioMusicas : String;
    diretorioFotos: String;
    fotopos : integer;
    falar : TSpeak;
    lastHour : TTime;
  end;

var
  frmControle: TfrmControle;

implementation

{$R *.lfm}

{ TfrmControle }

//Chama Alarme
procedure TfrmControle.ChamaAlarme();
var
  ShortTimeFormat : string;
  formattedDate1,formattedDate2 : string;
begin
  if (frmAlarme = nil) then
  begin
    DateTimeToString(formattedDate1, 'hh:mm', lastHour);
    DateTimeToString(formattedDate2, 'hh:mm', now());
    if(formattedDate2 <> formattedDate1) then
    begin
         lastHour := now();
         frmAlarme := tfrmAlarme.create(self);
         frmAlarme.showmodal();
         frmAlarme.free;
         frmAlarme := nil;
    end;
  end;
end;

//Realiza controle de Alarmes ativos
procedure TfrmControle.checaAlarme();
begin
   //showmessage('trocou foto');
   if (frmDespertador.VerificaAlarme() <> 0 ) then
   begin
       ChamaAlarme();
   end;
end;

procedure TfrmControle.CarregaFotos();
begin
   // Fotos := TStringList.create();
   Fotos := FindAllFiles(diretorioFotos,'*.jpg;*.bmp;', true);
end;

procedure Tfrmcontrole.TrocaFoto();
begin
    if (fotopos < Fotos.Count ) then
    begin
       Image1.Picture.LoadFromFile(Fotos[fotopos]);
       inc(fotopos);
    end
    else
    begin
      fotopos := 0;
    end;

end;

procedure TfrmControle.checaFotos();
begin
   if (tempo <= now()) then
   begin
       tempo := now()+0.00064;
       TrocaFoto();

   end;

end;

//Reseta o horario atribuindo meia noite e um minuto
//Isso evita que se só houver um alarme no dia seguinte nao funcione.
procedure TfrmControle.ResetClock();
var
  formattedDate1 , formattedDate2: string;
begin
    formattedDate1 := '00:01';
    DateTimeToString(formattedDate2, 'hh:mm', now());
    if(formattedDate2 <> formattedDate1) then
    begin
         lastHour := now();
    end;
end;

procedure TfrmControle.Timer1Timer(Sender: TObject);
begin
  lbHoras.Caption:= TimeToStr(now());
  lbDia.Caption:= DateToStr(now());
  checaAlarme();
  checaFotos();
  //Reseta o relogio
  Resetclock();
end;

procedure TfrmControle.FormCreate(Sender: TObject);
begin
  falar := TSpeak.create();
  diretorio := ExtractFilePath(Application.ExeName);
  {$ifdef windows}
    diretorioMusicas := diretorio + 'musicas';
    diretorioFotos:= diretorio+'fotos';
  {$else}
    diretorioMusicas := diretorio + 'musicas';
    diretorioFotos:= diretorio+'fotos';
  {$endif}
  //Carrega despertador
  frmDespertador := TfrmDespertador.Create(self);
  Timer1.Enabled:= true;
  //dmDados := TDmdados.create(self);
  tempo := now();
  fotopos := 0;
  CarregaFotos();
  falar.falar('Sistema totalmente carregado!');
  lastHour  := now();
end;

procedure TfrmControle.FormShow(Sender: TObject);
begin

end;

procedure TfrmControle.Image1DblClick(Sender: TObject);
begin
  falar.falar('pois não');
end;

procedure TfrmControle.SpeedButton1Click(Sender: TObject);
begin
  frmMain := tfrmmain.Create(self);
  frmmain.showmodal();
  frmmain.Destroy;
end;

procedure TfrmControle.SpeedButton2Click(Sender: TObject);
begin
  Timer1.Enabled:=false;
  frmDespertador.ShowModal;
  Timer1.Enabled:=true;
end;

procedure TfrmControle.SpeedButton3Click(Sender: TObject);
begin

end;

procedure TfrmControle.SpeedButton4Click(Sender: TObject);
begin
  frmCameras := tfrmcameras.create(self);
  frmcameras.showmodal;
  frmcameras.Destroy;
end;

procedure TfrmControle.SpeedButton5Click(Sender: TObject);
begin

end;

procedure TfrmControle.SpeedButton6Click(Sender: TObject);
begin
   frmMultimidia := TfrmMultimidia.create(self);
   frmMultimidia.ShowModal;
   frmMultimidia.destroy;
end;

end.

