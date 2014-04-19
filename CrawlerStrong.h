#ifndef CRAWLER_STRONG_H_
#define CRAWLER_STRONG_H_

#include "GG_Renderable.h"
#include "GG_Common.h"
#include "Crawler.h"

class CrawlerStrong : public Crawler {

private:
	GG::Renderable*             mIdleNonRenderable;
	GG::Renderable*             mWalkNonRenderable;

public:
	CrawlerStrong(float x, float y, bool jumpedOn);
	~CrawlerStrong();


	void                        Update(float dt) override;
	void						SetGrayscale(bool grayscale);
};

#endif