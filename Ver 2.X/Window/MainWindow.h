/*
* file: MainWindow.h
* info: Ӧ�ó��������ڶ��� ʹ��MiaoUI
*
* Author: Maplespe(mapleshr@icloud.com)
* 
* date: 2022-9-19 Create
*/
#pragma once
#include "..\framework.h"
#include <onnxruntime_cxx_api.h>
#include <Render\Sound\Mui_DirectSound.h>
#include <Render\Sound\Mui_AudioOgg.h>
namespace ttsUI
{
	class MioTTSControl;
	using namespace Mui;
	//�����ڳߴ�
	extern const UISize m_wndSize;
	//�����ڱ���
	extern std::wstring m_wndTitle;

	//��ʼ��and����ʼ��
	extern bool MiaoUI_Initialize(std::wstring& error);
	extern void MiaoUI_Uninitialize();

	//Ӧ�ó��������� WIndowsƽ̨
	class MainWindow : public Window::UIWindowsWnd
	{
	public:
		MainWindow(MRender* _render_) : UIWindowsWnd(_render_) {}
		~MainWindow();

		//�����¼��ص�
		virtual bool EventProc(UINotifyEvent, UIControl*, _m_param) override;
		//�����¼�Դ WIndows�����¼��ص�
		virtual _m_result EventSource(_m_param, _m_param) override;

		void setTTSCon(MioTTSControl*&& _ttsCon)
		{
			ttsCon = _ttsCon;
		}
	private:
		bool AfterCreated();
		bool CreateControls(UIControl* root);
		MioTTSControl* ttsCon = nullptr;
		MHelper::MuiXML* m_uiXML = nullptr;
		MFontLoader* m_fontLoader = nullptr;
		friend void MainWindow_CreateLoop();
	};

	struct modelInfo
	{
		std::wstring Folder;
		std::wstring Name;
		std::wstring Type;
		std::wstring Symbol;
		std::wstring Cleaner;
		std::vector<std::wstring> Speakers;
	};

	enum class modelType
	{
		Taco,
		Vits,
		Vitm
	};

	//�ؼ�����
	class MioTTSControl
	{
	public:
		MioTTSControl() = delete;
		MioTTSControl(ttsUI::MainWindow*&& _main)
		{
			_mainWindow = _main;
			_uiControl = _main->GetRootControl();
			hWnd = (HWND)(_main->GetWindowHandle());
		}
		UIProgressBar* process = nullptr;      //ת������
		UIBlurLayer* titlebar = nullptr;       //������
		UIButton* title_close = nullptr;       //�رհ�ť
		UIButton* title_minisize = nullptr;    //��С����ť
		UILabel* title_label = nullptr;        //����
		IconButton* tts_begin = nullptr;       //��ʼת����ť
		UIEditBox* wave_editbox = nullptr;     //�����ַ���
		UIComBox* modlist_main = nullptr;      //ģ���б�
		UIComBox* modlist_child = nullptr;     //˵����
		IconButton* modlist_refresh = nullptr; //ˢ���б�
		IconButton* modlist_folder = nullptr;  //��ģ���ļ���
		UILabel* voice_timecur = nullptr;      //��ǰ����ʱ��
		UILabel* voice_timeall = nullptr;      //��Ƶʱ��
		UIButton* voice_play = nullptr;        //���Ű�ť
		IconButton* voice_save = nullptr;      //���水ť
		UISlider* voice_volume = nullptr;      //��������
		UILabel* volume_text = nullptr;        //����
		UIImgBox* voice_imgbox = nullptr;      //ģ������
		UILabel* voice_imgname = nullptr;      //ģ����
		UISlider* voice_prog = nullptr;        //���Ž�����
		IconButton* clean_begin = nullptr;     //���������ı�
		Render::MDS_AudioPlayer* wavPlayer = nullptr;
		Render::MAudioTrack* wavTrack = nullptr;
		friend class MainWindow;
		//����ģ��
		void loadmodel();
		//ע���������¼�
		void RegisterLeftClickEvent(std::wstring&& _control, std::function<bool()>&& _fun) {
			_levents.insert(std::pair<std::wstring, std::function<bool()>>(_control, _fun));
		}
		//ע����ֵ�¼�
		void RegisterValEvent(std::wstring&& _control, std::function<bool(uint64_t)>&& _fun) {
			_sevents.insert(std::pair<std::wstring, std::function<bool(uint64_t)>>(_control, _fun));
		}
		//ע��ؼ�
		void RegisterControl();
		//ע���¼�
		void RegisterEvent();
		//��ʼ���ؼ�
		void setConsDefalut() const;
		//���ģ��
		void ClearModel();
		//��ʼ��
		void InitControl();
		//��ʼ��
		void InferQuit() const;
		//Tacotron2
		void InferTaco2(const std::wstring& _input);
		//InferVits
		void InferVits(const std::vector<std::wstring>& _input);
		//InferVitsM
		void InferVitsM(const std::vector<std::wstring>& _input,int chara);
		//Message
		void MessageCreate(const std::wstring& _input) const
		{
			MessageBox(hWnd, _input.c_str(), L"", MB_OK);
		}
		//cleaner
		std::wstring getCleanerStr(std::wstring& _input) const;

		void setSoundModule(bool Switch) const
		{
			voice_play->SetEnabled(Switch);
			voice_save->SetEnabled(Switch);
			voice_prog->SetEnabled(Switch);
		}
	protected:
		std::map <std::wstring, std::function<bool()>> _levents;
		std::map <std::wstring, std::function<bool(uint64_t)>> _sevents;
		std::vector<modelInfo> _models;
		std::vector<short> _wavData;
		std::wstring _curCleaner;
		MainWindow* _mainWindow = nullptr;
		UIControl* _uiControl = nullptr;
		HWND hWnd = nullptr;
		modelType _modelType = modelType::Taco;
		std::map<wchar_t, int64_t> _symbol;
		Ort::Session* sessionEncoder = nullptr;
		Ort::Session* sessionDecoderIter = nullptr;
		Ort::Session* sessionPostNet = nullptr;
		Ort::Session* sessionGan = nullptr;
		/*
		Ort::Session* sessionDec = nullptr;
		Ort::Session* sessionDp = nullptr;
		Ort::Session* sessionEnc_p = nullptr;
		Ort::Session* sessionFlow = nullptr;
		Ort::Session* sessionEmb = nullptr;
		*/
		Ort::Env* env = nullptr;
		Ort::SessionOptions* session_options = nullptr;
		Ort::MemoryInfo* memory_info = nullptr;
		bool playstat = false;
	};
	
	//����������
	extern void MainWindow_CreateLoop();
}


