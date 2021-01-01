object frmMain: TfrmMain
  Left = 192
  Top = 124
  Width = 298
  Height = 465
  Caption = 'Gestor de Rele Remoto'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 72
    Top = 16
    Width = 27
    Height = 13
    Caption = 'Reles'
  end
  object btRele01: TButton
    Left = 72
    Top = 72
    Width = 75
    Height = 25
    Caption = 'Rele01'
    TabOrder = 0
    OnClick = btRele01Click
  end
  object btRele02: TButton
    Left = 72
    Top = 112
    Width = 75
    Height = 25
    Caption = 'Rele02'
    TabOrder = 1
    OnClick = btRele02Click
  end
  object btRele03: TButton
    Left = 72
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Rele03'
    TabOrder = 2
    OnClick = btRele03Click
  end
  object btRele04: TButton
    Left = 72
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Rele04'
    TabOrder = 3
    OnClick = btRele04Click
  end
  object Memo1: TMemo
    Left = 0
    Top = 338
    Width = 282
    Height = 89
    Align = alBottom
    Lines.Strings = (
      'Memo1')
    TabOrder = 4
  end
  object Button1: TButton
    Left = 72
    Top = 232
    Width = 75
    Height = 25
    Caption = '<'
    TabOrder = 5
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 176
    Top = 232
    Width = 75
    Height = 25
    Caption = '>'
    TabOrder = 6
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 128
    Top = 272
    Width = 75
    Height = 25
    Caption = 'Para'
    TabOrder = 7
    OnClick = Button3Click
  end
  object ClientSocket1: TClientSocket
    Active = False
    Address = '192.168.1.177'
    ClientType = ctNonBlocking
    Port = 23
    OnConnecting = ClientSocket1Connecting
    OnConnect = ClientSocket1Connect
    OnRead = ClientSocket1Read
    OnError = ClientSocket1Error
    Left = 16
    Top = 48
  end
end
