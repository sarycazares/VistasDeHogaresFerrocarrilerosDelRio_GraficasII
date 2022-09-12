#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"

#define DEG_TO_RAD 0.0174532925f

class DXRR{	

private:
	int ancho;
	int alto;
public:	
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	int frameBillboard;

	TerrenoRR *terreno;
	TerrenoRR *water;
	SkyDome *skydome;
	BillboardRR *billboard;
	Camara *camara;
	ModeloRR* oxxo;
	ModeloRR* barrera;
	ModeloRR* elektra;
	ModeloRR* casa1;
	ModeloRR* banca;
	ModeloRR* banca2;
	ModeloRR* biblioteca;
	ModeloRR* bici;
	ModeloRR* brick;
	ModeloRR* edificio;
	ModeloRR* edificio2;
	ModeloRR* edificio3;
	ModeloRR* edificio4;
	ModeloRR* heladeria;
	ModeloRR* kiosko;
	ModeloRR* farmacia;
	ModeloRR* bodega;
	
	float izqder;
	float arriaba;
	float vel;
	bool breakpoint;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	XACTINDEX cueIndex;
	CXACT3Util m_XACT3;

	float anguloBici = 0.f;
	bool tercerapersona = false;
	float bicicletaTercerX = 0;
	float bicicletaTercerZ = 6;
	float bicicletacontador = 0;
	float bicicletacontadorControl = 0;
	bool existeBici = true;
	bool EsAgua = false;
	bool Musica;
	
    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		billCargaFuego();
		camara = new Camara(D3DXVECTOR3(0,80,6), D3DXVECTOR3(0,80,0), D3DXVECTOR3(0,1,0), Ancho, Alto);
		EsAgua = false;
		terreno = new TerrenoRR(300, 300, d3dDevice, d3dContext, L"Imagenes/TexturasTerreno/Montaña.png", L"Imagenes/TexturasTerreno/heightmap02.png", EsAgua);
		EsAgua = true;
		water = new TerrenoRR(300, 300, d3dDevice, d3dContext, L"Imagenes/TexturasAgua/water.jpg", L"Imagenes/TexturasAgua/water01.jpg", EsAgua);

		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Imagenes/TexturasSkydome/SkyDomeAmanecer.png");
		//Instancia de billboards
		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);
		//Instancia de modelos
		oxxo = new ModeloRR(d3dDevice, d3dContext, "Modelos/oxxo/oxxo.obj", L"Modelos/oxxo/oxxo_d.png", L"Modelos/oxxo/oxxo_d.png", 12.6, 94.3);
		barrera = new ModeloRR(d3dDevice, d3dContext, "Modelos/barrera nudillos/barrera.obj", L"Modelos/barrera nudillos/texturabarrera.png", L"Modelos/barrera nudillos/texturabarrera.png", 113.8, -72);
		casa1 = new ModeloRR(d3dDevice, d3dContext, "Modelos/casa1/casita 1.obj", L"Modelos/casa1/house-low-texture.jpg", L"Modelos/casa1/house-low-texture.jpg", 95.4, 0);
		kiosko = new ModeloRR(d3dDevice, d3dContext, "Modelos/kiosko/Graffiti_kiosk.obj", L"Modelos/kiosko/Diffuse_graffiti.png", L"Modelos/kiosko/Diffuse_graffiti.png", 93, -35);
		heladeria = new ModeloRR(d3dDevice, d3dContext, "Modelos/heladeria/heladeria.obj", L"Modelos/heladeria/Icecreambooth.jpeg", L"Modelos/heladeria/Icecreambooth.jpeg", 8, 9);
		elektra = new ModeloRR(d3dDevice, d3dContext, "Modelos/elektra/elektra.obj", L"Modelos/elektra/mx_elektra_d.png", L"Modelos/elektra/mx_elektra_d.png", 60, 97.3);
		edificio2 = new ModeloRR(d3dDevice, d3dContext, "Modelos/edificio 2/edificio 2.obj", L"Modelos/edificio 2/warewhouse_Diff.jpg", L"Modelos/edificio 2/warewhouse_Diff.jpg", 75.5, -79.5);
		brick = new ModeloRR(d3dDevice, d3dContext, "Modelos/brick/brick building.obj", L"Modelos/brick/brick-building.jpeg", L"Modelos/brick/brick-building.jpeg", 55.5, -78.5);
		//edificio4 = new ModeloRR(d3dDevice, d3dContext, "Modelos/brick/brick building.obj", L"Modelos/brick/brick-building.jpeg", L"Modelos/brick/brick-building.jpeg", 0, 0);
		banca = new ModeloRR(d3dDevice, d3dContext, "Modelos/Banca/Banca.obj", L"Modelos/Banca/TextuBanca.bmp", L"Modelos/Banca/TextuBanca.bmp", 12, -11.5);
		banca2 = new ModeloRR(d3dDevice, d3dContext, "Modelos/Banca/Banca.obj", L"Modelos/Banca/TextuBanca.bmp", L"Modelos/Banca/TextuBanca.bmp", 4, -11.5);
		bici = new ModeloRR(d3dDevice, d3dContext, "Modelos/Bici/Bici.obj", L"Modelos/Bici/bike.png", L"Modelos/Bici/bike.png", 0, 0);
	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		//MUSICA CUMBIONA SABROSA PARA MANEJAR UN TRAILER EN LA CARRETERA A LAREDO

		// Initialize XACT3
		Musica = m_XACT3.Initialize();
		if (!Musica) return false;
		Musica = m_XACT3.LoadWaveBank(L"Musica\\Win\\WaveBank.xwb");
		if (!Musica) return false;
		Musica = m_XACT3.LoadSoundBank(L"Musica\\Win\\SoundBank.xsb");
		if (!Musica) return false;

		//// Play XACT3 cue
		XACTINDEX cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("CumbiasSabrosas");
		m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);

		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{

		
		static float watermov[4];
		watermov[0] += 0.005;

		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if( d3dContext == 0 )
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 5 ;
		camara->UpdateCam(vel, arriaba, izqder);
		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		EsAgua = false;
		terreno->Draw(camara->vista, camara->proyeccion, watermov, EsAgua);
		EsAgua = true;
		water->Draw(camara->vista, camara->proyeccion, watermov, EsAgua);
		//TurnOnAlphaBlending();
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-11, -78, 4, 5, uv1, uv2, uv3, uv4, frameBillboard);

		//TurnOffAlphaBlending();
		oxxo->Draw(camara->vista, camara->proyeccion, 4.5, camara->posCam, 10.0f, 179.07f, 'Y', 1);
		barrera->Draw(camara->vista, camara->proyeccion, 4.5, camara->posCam, 10.0f, 0.0f, 'A', 1.2);
		casa1->Draw(camara->vista, camara->proyeccion, 5.5, camara->posCam, 10.0f, 0.0f, 'A', 0.35);
		edificio2->Draw(camara->vista, camara->proyeccion, 5.5, camara->posCam, 10.0f, 179.07f, 'Y', 0.3);
		//kiosko->Draw(camara->vista, camara->proyeccion, 10, camara->posCam, 10.0f, 0.0f, 'A', 2);
		heladeria->Draw(camara->vista, camara->proyeccion, 5, camara->posCam, 10.0f, 179.07f, 'Y', 0.75);
		elektra->Draw(camara->vista, camara->proyeccion, 4, camara->posCam, 10.0f, 179.07f, 'Y', 1);
		brick->Draw(camara->vista, camara->proyeccion, 5.5, camara->posCam, 10.0f, 0.0f, 'A', 0.15);
		//edificio4->Draw(camara->vista, camara->proyeccion, 5.5, camara->posCam, 10.0f, 0.0f, 'A', 1);
		banca->Draw(camara->vista, camara->proyeccion, 6.1, camara->posCam, 10.0f, 0.0f, 'A', 0.055);
		banca2->Draw(camara->vista, camara->proyeccion, 6.1, camara->posCam, 10.0f, 0.0f, 'A', 0.055);

		
		//bici->Draw(camara->vista, camara->proyeccion, 6, camara->posCam, 10.0f, (anguloBici * DEG_TO_RAD), 'Y', 0.12, camara->posCam.x, ((camara->posCam.y)/2),camara->posCam.z, vel, (anguloBici * DEG_TO_RAD), izqder);

		if (existeBici == true) {

			if (tercerapersona == false) {
				bici->Draw(camara->vista, camara->proyeccion, 6, camara->posCam, 10.0f, (anguloBici * DEG_TO_RAD), 'Y', 0.12, camara->posCam.x, ((camara->posCam.y) / 2), camara->posCam.z, vel, (anguloBici * DEG_TO_RAD), izqder);
			}
			else {
				bici->Draw(camara->vista, camara->proyeccion, 6, camara->posCam, 10.0f, (anguloBici * DEG_TO_RAD), 'Y', 0.12, (camara->posCam.x + bicicletaTercerX), ((camara->posCam.y) / 2), (camara->posCam.z + bicicletaTercerZ), vel, (anguloBici * DEG_TO_RAD), izqder);
			}
		}
		else {
			bici->Draw(camara->vista, camara->proyeccion, 6, camara->posCam, 10.0f, 0.0f, 'A', 0.08);
		}


		swapChain->Present( 1, 0 );
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]));

		if (distance < sphere[2])
			collition = true;
		return collition;
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[0] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

};
#endif