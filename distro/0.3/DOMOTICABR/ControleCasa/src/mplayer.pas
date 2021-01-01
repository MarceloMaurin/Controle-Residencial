unit mplayer;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils;

type TMplayer = class(TObject)

 private
   { private declarations }
   ExeName : String;
   ExePath : String;
   DIRSOM : string;
   DIRMUSICA : String;

 public
   { public declarations }
   constructor create();
   procedure falar(Texto : string);
end;

implementation
uses Controle;


constructor TMplayer.create();
begin
   {$ifdef windows}
     ExeName := 'wmplayer.exe';
     ExePath :=  'C:\%ProgramFiles(x86)%\Windows Media Player\';
     DirSom := 'C:\Users\mmm\Documents\projetos\Controle Residencial\windows\controle de casa\soms\';
     DirMusica := 'C:\Users\mmm\Documents\projetos\Controle Residencial\windows\controle de casa\musicas';
   {$else}   //linux
     ExeName := 'sh_som';
     ExePath :=  frmcontrole.diretorio;
     DirSom := '/projetos/Controle Residencial/windows/controle de casa/soms/';
     DirMusica := '/projetos/Controle Residencial/windows/controle de casa/musicas';
     ExePath :=  frmcontrole.diretorio+'bin\';
   {$endif}
end;

procedure TMplayer.falar(Texto : string);
begin
   {$ifdef windows}
   ExecuteProcess(ExePath+ExeName,'-vpt -p65 -s120 "'+Texto + '"',[]);
   {$else}
   ExecuteProcess(ExePath+ExeName,'"'+Texto + '"',[]);
   {$endif}
end;



end.

