#include "Plane.h"
#include "ResourceManager.h"
#include "InkPlaneRender.h"

Plane::Plane(std::unique_ptr<TexturePack> pTexturePack, std::unique_ptr<Material> pMaterial) :
	Shape(ResourceManager::instance()->loadModel("plane.obj"), std::move(pTexturePack), std::move(pMaterial), std::make_unique<InkPlaneRender>())
{
}

Plane::Plane() :
	Shape(ResourceManager::instance()->loadModel("plane.obj"), nullptr, nullptr, std::make_unique<InkPlaneRender>())
{
}

//TODO: Source: http://www.r-5.org/files/books/computers/algo-list/realtime-3d/Christer_Ericson-Real-Time_Collision_Detection-EN.pdf

void Plane::collideWith(Particle pParticle)
{
	//Assuming that the planes do not move
	const auto matrix = DirectX::XMLoadFloat4x4(&mCurrentMatrix);

	auto planeCenter = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	auto planeNormal = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	planeCenter = DirectX::XMVector3Transform(planeCenter, matrix);
	planeNormal = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMVector3Transform(planeNormal, matrix), planeCenter));

	const auto tempStart = pParticle.getPreviousPosition();
	const auto tempEnd = pParticle.getCurrentPosition();
	
	const auto particleStart = DirectX::XMLoadFloat3(&tempStart);
	const auto particleEnd = DirectX::XMLoadFloat3(&tempEnd);

	const auto particleDirection = DirectX::XMVectorSubtract(particleEnd, particleStart);

	const auto planeDot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeNormal, planeCenter));

	const auto time = (planeDot - DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeNormal, particleStart))) / DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeNormal, particleDirection));

	if (time >= 0.0f && time <= 1.0f)
	{
		const auto tempPosition = DirectX::XMVectorAdd(particleStart, DirectX::XMVectorScale(particleDirection, time));

		auto planeTangent = DirectX::XMVectorSet(1.0f, 0.0, 0.0, 1.0f);
		auto planeBiTangent = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

		planeTangent = DirectX::XMVectorSubtract(DirectX::XMVector3Transform(planeTangent, matrix), planeCenter);
		planeBiTangent = DirectX::XMVectorSubtract(DirectX::XMVector3Transform(planeBiTangent, matrix), planeCenter);

		const auto tangentLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(planeTangent));
		const auto biTangentLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(planeBiTangent));

		planeTangent = DirectX::XMVector3Normalize(planeTangent);
		planeBiTangent = DirectX::XMVector3Normalize(planeBiTangent);

		const auto tangentDot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeTangent, DirectX::XMVectorSubtract(tempPosition, planeCenter)));
		const auto biTangentDot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeBiTangent, DirectX::XMVectorSubtract(tempPosition, planeCenter)));

		if (abs(tangentDot) <= tangentLength && abs(biTangentDot) <= biTangentLength)
		{
			//TODO: Add to manifold
			//tempPosition
			//time
			//
		}
	}
}