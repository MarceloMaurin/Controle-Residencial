unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil,
  Forms, Controls, Graphics, Dialogs, ExtCtrls, computador;

type

  { TfrmMain }

  TfrmMain = class(TForm)
    controladorQuarto1: TImage;
    controladorQuarto2: TImage;
    controladorQuarto3: TImage;
    controladorQuarto4: TImage;
    Image1: TImage;
    Image2: TImage;
    arcondicionado: TImage;
    controladorEscritorio: TImage;
    camerapanoramica: TImage;
    carro: TImage;
    Image3: TImage;
    Image4: TImage;
    Image5: TImage;
    luzquarto2: TImage;
    luzquarto1: TImage;
    luzescritorio: TImage;
    luzsala: TImage;
    luzexterna1: TImage;
    luzexterna2: TImage;
    luzexterna3: TImage;
    luzjantar: TImage;
    smovimentocopa: TImage;
    smovimentocorredor: TImage;
    smovimentocozinha: TImage;
    smovimentosala: TImage;
    stemperatura: TImage;
    stemperatura1: TImage;
    stemperatura2: TImage;
    wifiescritorio: TImage;
    wificozinha: TImage;
    wifiescritorio4: TImage;
    wifisala: TImage;
    wifiescritorio2: TImage;
    wifiescritorio3: TImage;
    procedure arcondicionadoClick(Sender: TObject);
    procedure carroClick(Sender: TObject);
    procedure controladorEscritorioClick(Sender: TObject);
    procedure Image3Click(Sender: TObject);
    procedure Image4Click(Sender: TObject);
    procedure luzescritorioClick(Sender: TObject);
    procedure luzexterna1Click(Sender: TObject);
    procedure luzexterna2Click(Sender: TObject);
    procedure luzexterna3Click(Sender: TObject);
    procedure luzjantarClick(Sender: TObject);
    procedure luzquarto1Click(Sender: TObject);
    procedure luzquarto2ChangeBounds(Sender: TObject);
    procedure luzcozinhaChangeBounds(Sender: TObject);
    procedure luzescritorioChangeBounds(Sender: TObject);
    procedure luzjantarChangeBounds(Sender: TObject);
    procedure luzquarto2Click(Sender: TObject);
    procedure luzsalaClick(Sender: TObject);
    procedure smovimentocopaClick(Sender: TObject);
    procedure smovimentocozinhaClick(Sender: TObject);
    procedure smovimentocorredorClick(Sender: TObject);
    procedure smovimentosalaClick(Sender: TObject);
    procedure stemperatura1Click(Sender: TObject);
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

procedure TfrmMain.carroClick(Sender: TObject);
begin
  ShowMessage('carro!');
end;

procedure TfrmMain.controladorEscritorioClick(Sender: TObject);
begin

end;

procedure TfrmMain.Image3Click(Sender: TObject);
begin
  ShowMessage('luz garagem');
end;

procedure TfrmMain.Image4Click(Sender: TObject);
begin
  frmComputador := TfrmComputador.create(self);
  frmComputador.showmodal;
  frmcomputador.destroy;
end;

procedure TfrmMain.luzescritorioClick(Sender: TObject);
begin
   ShowMessage('luz escritorio');
end;

procedure TfrmMain.luzexterna1Click(Sender: TObject);
begin
   ShowMessage('luz externa1');
end;

procedure TfrmMain.luzexterna2Click(Sender: TObject);
begin
  ShowMessage('luz externa2');
end;

procedure TfrmMain.luzexterna3Click(Sender: TObject);
begin
  ShowMessage('luz externa3');
end;

procedure TfrmMain.luzjantarClick(Sender: TObject);
begin
    ShowMessage('luz jantar');
end;

procedure TfrmMain.luzquarto1Click(Sender: TObject);
begin
   ShowMessage('luz quarto1');
end;

procedure TfrmMain.arcondicionadoClick(Sender: TObject);
begin
  ShowMessage('ar condicionado');
end;

procedure TfrmMain.luzquarto2ChangeBounds(Sender: TObject);
begin

end;

procedure TfrmMain.luzcozinhaChangeBounds(Sender: TObject);
begin

end;

procedure TfrmMain.luzescritorioChangeBounds(Sender: TObject);
begin
   ShowMessage('luz sala');
end;

procedure TfrmMain.luzjantarChangeBounds(Sender: TObject);
begin

end;

procedure TfrmMain.luzquarto2Click(Sender: TObject);
begin
   ShowMessage('luz quarto2');
end;

procedure TfrmMain.luzsalaClick(Sender: TObject);
begin
   ShowMessage('Luz interna');
end;

procedure TfrmMain.smovimentocopaClick(Sender: TObject);
begin
   ShowMessage('sensor movimento copa');
end;

procedure TfrmMain.smovimentocozinhaClick(Sender: TObject);
begin
   ShowMessage('sensor movimento cozinha');
end;

procedure TfrmMain.smovimentocorredorClick(Sender: TObject);
begin
  showMessage('Sensor movimento corredor');
end;

procedure TfrmMain.smovimentosalaClick(Sender: TObject);
begin
  showMessage('Sensor movimento sala');
end;

procedure TfrmMain.stemperatura1Click(Sender: TObject);
begin
   ShowMessage('sensor temperatura quarto1');
end;

end.

