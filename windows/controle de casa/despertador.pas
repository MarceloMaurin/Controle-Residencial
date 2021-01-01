unit Despertador;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  ComCtrls,dos;

type  RDespertador = record
   id : integer;
   dias : byte;
   hora : String[20];
   ativo : byte;
end;

type

  { TfrmDespertador }

  TfrmDespertador = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    CheckBox3: TCheckBox;
    CheckBox4: TCheckBox;
    CheckBox5: TCheckBox;
    CheckBox6: TCheckBox;
    CheckBox7: TCheckBox;
    ckAtivo: TCheckBox;
    Edit1: TEdit;
    Edit2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    lstDespertador: TListBox;
    UpDown1: TUpDown;
    UpDown2: TUpDown;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Edit2Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure lstDespertadorClick(Sender: TObject);
    procedure lstDespertadorDblClick(Sender: TObject);
    procedure UpDown1Click(Sender: TObject; Button: TUDBtnType);
    procedure UpDown1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure UpDown2Click(Sender: TObject; Button: TUDBtnType);
  private
    { private declarations }
  public
    { public declarations }
    Despertador : RDespertador;
    diretoriodespertador : string;
    arquivodespertador : string;
    procedure Carregar();
    procedure PegarValor();
    procedure Atualizar();
    function VerificaAlarme() : integer;
  end;

var
  frmDespertador: TfrmDespertador;

implementation

{$R *.lfm}

{ TfrmDespertador }
uses controle;

function TfrmDespertador.VerificaAlarme() : integer;
var
  a, resultado : integer;
  Hora , Min, Seg, HSeg : word;
  horareg, minReg : integer;

begin
  resultado := 0;
  for a:= 0 to lstDespertador.Count-1 do
  begin
      lstDespertador.ItemIndex:= a;
      Carregar();
      //Verifica
      GETTime( Hora, min,seg,HSeg);
      horareg := strtoint( Edit1.text);
      minreg := strtoint(Edit2.text);
      if (horareg = Hora) and  (min = minreg) then
      begin
         resultado := a;
         break;
      end;

  end;
  result :=resultado;
end;

procedure TfrmDespertador.UpDown1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TfrmDespertador.UpDown2Click(Sender: TObject; Button: TUDBtnType);
begin
   if (button = btNext) then
     begin
       if (strtoint(edit2.text) <59) then
       begin
         edit2.Text:=inttostr(strtoint(edit2.Text)+1);
       end
       else
       begin
        edit2.Text:= '0';
       end;
     end
     else
     begin
       if (strtoint(edit2.text) > 0) then
       begin
            edit2.Text:=inttostr(strtoint(edit2.Text)-1);
       end
       else
       begin
         edit2.Text:= '59';
       end;
     end;
end;

procedure TfrmDespertador.UpDown1Click(Sender: TObject; Button: TUDBtnType);
begin
   if (button = btNext) then
   begin
     if (strtoint(edit1.text) < 23) then
     begin
       edit1.Text:=inttostr(strtoint(edit1.Text)+1);
     end
     else
     begin
      edit1.Text:= '0';
     end;
   end
   else
   begin
     if (strtoint(edit1.text) > 0) then
     begin
          edit1.Text:=inttostr(strtoint(edit1.Text)-1);
     end
     else
     begin
       edit1.Text:= '23';
     end;
   end;
end;

procedure TfrmDespertador.Edit2Click(Sender: TObject);
begin

end;

procedure TfrmDespertador.Atualizar();
begin
  if (FileExistsUTF8(arquivodespertador)= true) then
  begin
    lstDespertador.Items.LoadFromFile(arquivodespertador);
  end
  else
  begin
    lstDespertador.Items.SaveToFile(arquivodespertador);
  end;
end;

procedure TfrmDespertador.FormCreate(Sender: TObject);
begin
  diretoriodespertador := frmControle.diretorio;
  {$ifdef windows}
    arquivodespertador := diretoriodespertador +'despertador.conf';
  {$else}
    arquivodespertador := diretoriodespertador +'despertador.conf';
  {$endif}
  Atualizar();
end;

procedure TfrmDespertador.FormDestroy(Sender: TObject);
begin
  //frmDespertador.Free;
end;

procedure TfrmDespertador.lstDespertadorClick(Sender: TObject);
begin
    Carregar();
end;

procedure TfrmDespertador.Carregar();
  var
  registro : integer;
  info, info2 : string;
  posicaov : integer;
  lista : string;
  valor  : integer;
begin
   registro := 0;
   lista := lstDespertador.Items.Strings[lstDespertador.ItemIndex];

   posicaov := pos( ',',lista);
   info := Copy(lista,1,posicaov-1);
   valor := strtoint(info);
   CheckBox1.Checked := ((valor and 1)= 1);
   CheckBox2.Checked := ((valor and 2)= 2);
   CheckBox3.Checked := ((valor and 4)= 4);
   CheckBox4.Checked := ((valor and 8)= 8);
   CheckBox5.Checked := ((valor and 16)= 16);
   CheckBox6.Checked := ((valor and 32)= 32);
   CheckBox7.Checked := ((valor and 64)= 64);

   //Segundo paramentro
   lista := copy(lista,posicaov+1,length(lista));
   posicaov := pos(',',lista);
   info := Copy(lista,1,posicaov-1);
   valor := strtoint(info);
   edit1.text := inttostr(valor);

   //terceiro paramentro
   lista := copy(lista,posicaov+1,length(lista));
   posicaov := pos( ',', lista);
   info := Copy(lista,1,posicaov-1);
   valor := strtoint(info);
   edit2.text := inttostr(valor);

   //quarto paramentro
   lista := copy(lista,posicaov+1,length(lista));
   posicaov := length(lista);
   info := lista;
   valor := strtoint(info);
   ckAtivo.Checked := valor =1;

end;


procedure TfrmDespertador.lstDespertadorDblClick(Sender: TObject);
  var
    info : string;
    registro : integer;

  begin

     info:= lstDespertador.Items.Strings[lstDespertador.ItemIndex];
     registro := 0;
     info := '';
     if CheckBox1.Checked = true then registro := registro + 1;
     if CheckBox2.Checked = true then registro := registro + 2;
     if CheckBox3.Checked = true then registro := registro + 4;
     if CheckBox4.Checked = true then registro := registro + 8;
     if CheckBox5.Checked = true then registro := registro + 16;
     if CheckBox6.Checked = true then registro := registro + 32;
     if CheckBox7.Checked = true then registro := registro + 64;
     info := info + inttostr(registro)+',';
     info := info + Edit1.text +',';
     info := info + Edit2.text +',';
     if ckAtivo.Checked then
       info := info + '1'
     else
       info := info + '0';
     lstDespertador.Items.Append(info);
     lstDespertador.Items.SaveToFile(arquivodespertador);

  end;

procedure TfrmDespertador.PegarValor();
  var
    registro : integer;
    info : string;
    valor : RDespertador;
begin
    registro := 0;
     info := '';
     if CheckBox1.Checked = true then registro := registro + 1;
     if CheckBox2.Checked = true then registro := registro + 2;
     if CheckBox3.Checked = true then registro := registro + 4;
     if CheckBox4.Checked = true then registro := registro + 8;
     if CheckBox5.Checked = true then registro := registro + 16;
     if CheckBox6.Checked = true then registro := registro + 32;
     if CheckBox7.Checked = true then registro := registro + 64;

     info := info + inttostr(registro)+',';
     info := info + Edit1.text +',';
     info := info + Edit2.text +',';
     if ckAtivo.Checked then
       info := info + '1'
     else
       info := info + '0';
     lstDespertador.Items.Strings[lstDespertador.ItemIndex] := info;
end;

procedure TfrmDespertador.Button1Click(Sender: TObject);
  var
    registro : integer;
    info : string;
  begin
     PegarValor();
     lstDespertador.Items.SaveToFile(arquivodespertador);
     Carregar();
     close;
  end;

procedure TfrmDespertador.Button2Click(Sender: TObject);
begin
  close;
end;

procedure TfrmDespertador.Button3Click(Sender: TObject);
var
  registro : integer;
  info : string;
begin
   info := '0,0,0,0';
   lstDespertador.Items.Append(info);
   lstDespertador.Items.SaveToFile(arquivodespertador);
end;

procedure TfrmDespertador.Button4Click(Sender: TObject);
begin
  lstDespertador.Items.Delete(lstDespertador.ItemIndex);
  lstDespertador.Items.SaveToFile(arquivodespertador);
end;

end.

