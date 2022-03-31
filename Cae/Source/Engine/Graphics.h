#pragma once
#include <wrl.h>
#include <d3d11.h>

namespace Cae {
	class Graphics {
	public:
		Graphics(HWND hWnd, int width, int height);
		~Graphics();

		void RenderFrame();
	private:
		UINT width;
		UINT height;
		D3D_FEATURE_LEVEL g_FeatureLevel;

		Microsoft::WRL::ComPtr<ID3D11Device>			g_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			g_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		g_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	g_BackBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	g_DepthStencilView;
	};
}