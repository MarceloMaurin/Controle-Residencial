unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ScktComp;

type
  TfrmMain = class(TForm)
    ClientSocket1: TClientSocket;
    Label1: TLabel;
    btRele01: TButton;
    btRele02: TButton;
    btRele03: TButton;
    btRele04: TButton;
    Memo1: TMemo;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure btRele01Click(Sender: TObject);
    procedure btRele02Click(Sender: TObject);
    procedure btRele03Click(Sender: TObject);
    procedure btRele04Click(Sender: TObject);
    procedure ClientSocket1Read(Sender: TObject; Socket: TCustomWinSocket);
    procedure ClientSocket1Connecting(Sender: TObject;
      Socket: TCustomWinSocket);
    procedure ClientSocket1Connect(Sender: TObject;
      Socket: TCustomWinSocket);
    procedure ClientSocket1Error(Sender: TObject; Socket: TCustomWinSocket;
      ErrorEvent: TErrorEvent; var ErrorCode: Integer);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

procedure TfrmMain.FormShow(Sender: TObject);
begin
   //ClientSocket1.Active := true;
   ClientSocket1.Open;
   Memo1.Lines.Clear;
end;

procedure TfrmMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
 ClientSocket1.Active := false;
end;

procedure TfrmMain.btRele01Click(Sender: TObject);
begin
 ClientSocket1.Socket.SendText('1');
end;

procedure TfrmMain.btRele02Click(Sender: TObject);
begin
   ClientSocket1.Socket.SendText('2');
end;

procedure TfrmMain.btRele03Click(Sender: TObject);
begin
 ClientSocket1.Socket.SendText('3');
end;

procedure TfrmMain.btRele04Click(Sender: TObject);
begin
 ClientSocket1.Socket.SendText('4');
end;

procedure TfrmMain.ClientSocket1Read(Sender: TObject;
  Socket: TCustomWinSocket);
begin
 Memo1.Lines.Append(Socket.ReceiveText);
end;

procedure TfrmMain.ClientSocket1Connecting(Sender: TObject;
  Socket: TCustomWinSocket);
begin
  Memo1.Lines.Append('Tentando conectar no client');
end;

procedure TfrmMain.ClientSocket1Connect(Sender: TObject;
  Socket: TCustomWinSocket);
begin
 memo1.Lines.Append('Conectou no cliente');
end;

procedure TfrmMain.ClientSocket1Error(Sender: TObject;
  Socket: TCustomWinSocket; ErrorEvent: TErrorEvent;
  var ErrorCode: Integer);
begin

  Memo1.Lines.Append('Erro ao tentar conectar');
end;

procedure TfrmMain.Button1Click(Sender: TObject);
begin
     ClientSocket1.Socket.SendText('<');
end;

procedure TfrmMain.Button2Click(Sender: TObject);
begin
 ClientSocket1.Socket.SendText('>');
end;

procedure TfrmMain.Button3Click(Sender: TObject);
begin
 ClientSocket1.Socket.SendText(' ');
end;

end.
