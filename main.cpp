#include <iostream>
#include <string>

#include <openvr.h>

#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include <Windows.h>

#include <gl\GL.h>


bool ConnectToVRRuntime()
{
	auto m_eLastHmdError = vr::VRInitError_None;

	vr::IVRSystem *pVRSystem = vr::VR_Init(&m_eLastHmdError, vr::VRApplication_Overlay);

	if (m_eLastHmdError != vr::VRInitError_None)
	{
		std::cout << "No Driver" << std::endl;
		std::cout << "No Display" << std::endl;
		return false;
	}

	std::cout << "Status: " << m_eLastHmdError << std::endl;

	return true;
}


int main(int argc, char *argv[])
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "VR Chatoverlay", sf::Style::Close);

	sf::RenderTexture texture;
	texture.create(256, 256, true);

	sf::CircleShape ball;
	ball.setFillColor(sf::Color::White);
	ball.setRadius(25);
	ball.setPosition(0.0, 0.0);

	window.setFramerateLimit(10);

	std::string m_strName = "overlay";
	bool compositor = vr::VRCompositor() != NULL;
	bool bSuccess = true;

	vr::VROverlayHandle_t m_ulOverlayHandle;
	vr::VROverlayHandle_t m_ulOverlayThumbnailHandle;

	ConnectToVRRuntime();

	if (vr::VROverlay())
	{
		std::cout << "vr::VROverlay() == true" << std::endl;
		std::string sKey = std::string("chat.") + m_strName;
		//vr::VROverlayError overlayError = vr::VROverlay()->CreateDashboardOverlay(sKey.c_str(), m_strName.c_str(), &m_ulOverlayHandle, &m_ulOverlayThumbnailHandle);
		vr::VROverlayError overlayError = vr::VROverlay()->CreateOverlay(sKey.c_str(), m_strName.c_str(), &m_ulOverlayHandle);
		bSuccess = bSuccess && overlayError == vr::VROverlayError_None;
	}

	if (bSuccess)
	{
		std::cout << "Success" << std::endl;
		//vr::VROverlay()->SetOverlayWidthInMeters(m_ulOverlayHandle, 1.5f);
		//vr::VROverlay()->SetOverlayInputMethod(m_ulOverlayHandle, vr::VROverlayInputMethod_None);
	}

	//Better Quality Rendering? (According to the Docs there can only be one overlay using this and it seems the camera is using it?)
	//vr::VROverlay()->SetHighQualityOverlay(m_ulOverlayHandle);


	//Used for DashboardOverlays
	//vr::VROverlay()->ShowDashboard("chat.overlay");


	//List all tracked devices (HMD; Controllers; Basestations)
	//Sometimes the Controllers are #1 and #2 sometimes they're #3 and #4 for me
	//You could even have Overlays attached to the Lighthouses
	//You can also have the overlay placed in absolute space although it's a bit different
	for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		vr::TrackedDeviceClass dclass = vr::VRSystem()->GetTrackedDeviceClass(i);

		switch (dclass)
		{
		case vr::TrackedDeviceClass_Controller:
		{
			std::cout << i << " = Controller" << std::endl;
		}
		break;

		case vr::TrackedDeviceClass_HMD:
		{
			std::cout << i << " = HMD" << std::endl;
		}
		break;

		case vr::TrackedDeviceClass_Other:
		{
			std::cout << i << " = Other" << std::endl;
		}
		break;

		case vr::TrackedDeviceClass_Invalid:
		{
			std::cout << i << " = Invalid" << std::endl;
		}
		break;
		}
	}


	//Horrible at this stuff but this centers the Overlay on the Tracked Device
	vr::HmdMatrix34_t *transform = new vr::HmdMatrix34_t;

	transform->m[0][0] = 1;
	transform->m[0][1] = 0;
	transform->m[0][2] = 0;
	transform->m[0][3] = 0; //x

	transform->m[1][0] = 0;
	transform->m[1][1] = 1;
	transform->m[1][2] = 0;
	transform->m[1][3] = 0; //y

	transform->m[2][0] = 0;
	transform->m[2][1] = 0;
	transform->m[2][2] = 1;
	transform->m[2][3] = 0; //z


	vr::VROverlayTransformType *ttype = new vr::VROverlayTransformType;
	vr::VROverlay()->GetOverlayTransformType(m_ulOverlayHandle, ttype);

	//Attach the overlay relative to the tracked device
	vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulOverlayHandle, 1, transform);


	//Debugging, docs say if you want to read (GetOverlayTransform...) from the wrong type (Absolute, Relativ) you'll get errors
	if (*ttype == vr::VROverlayTransform_Absolute)
	{
		std::cout << "Absolute" << std::endl;
	}
	else if (*ttype == vr::VROverlayTransform_TrackedDeviceRelative)
	{
		std::cout << "Relative" << std::endl;
	}

	//I have no idead what I'm doing
	GLuint texture_handle = 0;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture_handle);
	glBindTexture(GL_TEXTURE_2D, texture_handle);


	//You can increase or decrase the size of the overlay
	vr::VROverlay()->SetOverlayWidthInMeters(m_ulOverlayHandle, 0.5);
	//And the alpha :)
	vr::VROverlay()->SetOverlayAlpha(m_ulOverlayHandle, 0.5);

	//Make the overlay visible
	vr::VROverlay()->ShowOverlay(m_ulOverlayHandle);

	
	//SFML stuff
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		//Had problems with the overlay when not processing the event loop
		vr::VREvent_t vrEvent;
		while (vr::VROverlay()->PollNextOverlayEvent(m_ulOverlayHandle, &vrEvent, sizeof(vrEvent)))
		{
			//std::cout << "EVENT" << std::endl;

			switch (vrEvent.eventType)
			{
			case vr::VREvent_OverlayShown:
			{
				//m_pWidget->repaint();
				std::cout << "repaint" << std::endl;
			}
			break;

			case vr::VREvent_Quit:
			{
				return -1;
			}
			break;
			}
		}

		//What we're doing here is basically:
		//Draw to a rendertexture (offscreen)
		//Get the raw pixels of the rendertexture and feed it to openvr
		//Additionally draw the rendertexture to the window to display what we have drawn

		//SetOverlayFromFile works as you'd expect
		//SetOverlayTexture seems to be buggy BUT according to the docs is the recommended way (at least prefered over SetOverlayRaw)
		//SetOverlayRaw feed raw pixels + size to openvr for the overlay

		texture.clear();

		texture.draw(ball);

		texture.display();

		sf::Image img = texture.getTexture().copyToImage();

		//glTexImage2D(GL_TEXTURE_2D, 0, 3, img.getSize().x, img.getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, img.getPixelsPtr());
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.getSize().x, img.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		/*glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			img.getSize().x, img.getSize().y,
			0,
			GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr()
		);*/

		//vr::Texture_t vrtex = { (void *)texture_handle, vr::API_OpenGL, vr::ColorSpace_Auto };

		//vr::VROverlay()->SetOverlayFromFile(m_ulOverlayHandle, "D:\\ovr\\pic.png");
		//vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &vrtex);
		vr::VROverlay()->SetOverlayRaw(m_ulOverlayHandle, (void *)img.getPixelsPtr(), 256, 256, 4);
		//vr::VROverlay()->ShowOverlay(m_ulOverlayHandle);

		window.clear();

		sf::Sprite sprite(texture.getTexture());

		window.draw(sprite);
		ball.move(sf::Vector2f(2.0, 0.0));
		if (ball.getPosition().x > 256)
		{
			ball.setPosition(0.0, 0.0);
		}

		window.display();
	}

	return 0;
}
