#include <Ctrl 2kPCH.h>

/*

particle engine code

*/

ParticleEngine::ParticleEngine(CoreEngine& engine) :
	parentEngine(engine)
{
	//cstr
}

void ParticleEngine::spawnEffect()
{

}

void ParticleEngine::addEffect(ParticleEffect *pfx)
{
	this->m_buffer.push_back(pfx);
	this->m_bufferSize = this->m_buffer.size();
}

void ParticleEngine::update()
{
	for (int i = 0; i < this->m_buffer.size(); i++) //iterates and checks/removes any dead particles
	{
		if (this->m_buffer[i] != nullptr) //check that it is not updating a nullptr
		{
			this->parentEngine.getPhysicsEngine().updateParticleFX(this->m_buffer[i]);

			if (this->m_buffer[i]->isDead()) //removes dead particles
			{
				delete this->m_buffer[i];
				this->m_buffer[i] = nullptr;
				this->m_buffer.erase(this->m_buffer.begin() + i);
				i--;
			}
		}
		else //delete useless ptr if needed
		{
			this->m_buffer.erase(this->m_buffer.begin() + i);
			i--;
		}
	}
}

void ParticleEngine::render()
{
	RenderEngine& rEngine = this->parentEngine.getRenderEngine();
	for (int i = 0; i < this->m_buffer.size(); i++) //iterates and checks/removes any dead particles
	{
		if (this->m_buffer[i] != nullptr) //check that it is not updating a nullptr
		{
			rEngine.drawParticleFX(this->m_buffer[i]);
		}
	}
}

ParticleEngine::~ParticleEngine()
{
	//dstr
}

/*

end ParticleEngine

*/