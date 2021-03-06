#ifndef CRAWLER_WEAK_H_
#define CRAWLER_WEAK_H_

#include "GG_Renderable.h"
#include "GG_Common.h"
#include "Crawler.h"

class CrawlerWeak : public Crawler {

public:
	CrawlerWeak(float x, float y, bool jumpedOn);
	CrawlerWeak::~CrawlerWeak(){}

	void                        Update(float dt) override;
	void						SetGrayscale(bool grayscale);
};

#endif