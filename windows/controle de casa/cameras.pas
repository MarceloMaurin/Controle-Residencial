unit Cameras;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls;

type

  { TfrmCameras }

  TfrmCameras = class(TForm)
    Image1: TImage;
    Image2: TImage;
    Image3: TImage;
    Image4: TImage;
    Shape1: TShape;
    Shape2: TShape;
    Shape3: TShape;
    Shape4: TShape;
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  frmCameras: TfrmCameras;

implementation

{$R *.lfm}

end.

