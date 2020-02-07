#include <core/scalar.h>
#include <core/image.h>
#include <core/random.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <rt/cameras/camera.h>
#include <iostream>
#include <rt/integrators/integrator.h>
#include <future>
#include <vector>
#include<functional>



namespace rt {

Renderer::Renderer(Camera* cam, Integrator* integrator)
    : cam(cam), integrator(integrator), samples(1)
{}

void Renderer::render(Image& img) {
    //iterate over every pixel in the image
	
	std::size_t numPixels = img.width() * img.height();
	std::size_t numThreads = std::thread::hardware_concurrency();
	volatile std::atomic<std::size_t> count(0);
	std::vector<std::future<void>> promoses;

	

	while(numThreads --){
		promoses.emplace_back(
			std::async(
				[=,&count, &img](){
					while(true){
						std::size_t index = count++;
						if(index >= numPixels){
							break;
						}

						uint i = index % img.width();
						uint j = index / img.width();
						std::cout<<j<<std::endl;
						RGBColor accumulator = RGBColor::rep(0.f);
						//accounting for supersampling
						std::vector<std::future<RGBColor>> colors;
						for(uint k = 0; k < this->samples; k++){
							//getting the normalized device coords [0, 1]
							float ndcx;
							float ndcy;
							if(k>0){
								ndcx = (float(i) + random()) / float(img.width());
								ndcy = (float(j) + random()) / float(img.height());
							}
							else{
								ndcx = (float(i) + 0.5f) / float(img.width());
								ndcy = (float(j) + 0.5f) / float(img.height());
							}

							//getting the screen space coordinates [-1, 1]
							float sscx = (ndcx * 2.0f) - 1.0f;
							float sscy = (ndcy * 2.0f) - 1.0f;

							//generating the ray from the camera
							Ray ray = cam->getPrimaryRay(sscx, -sscy);
							accumulator = accumulator + integrator->getRadiance(ray);
						}

						img(i,j) = accumulator/(float)this->samples;
					}
				}
			)
		);
	}


 
	// for(uint i = 0; i < img.width(); i++){
	// 	for(uint j = 0; j< img.height(); j++){

	// 		RGBColor accumulator = RGBColor::rep(0.f);
	// 		//accounting for supersampling
	// 		std::vector<std::future<RGBColor>> colors;
	// 		for(uint k = 0; k < this->samples; k++){
	// 			//getting the normalized device coords [0, 1]
	// 			float ndcx;
	// 			float ndcy;
	// 			if(k>0){
	// 				ndcx = (float(i) + random()) / float(img.width());
	// 				ndcy = (float(j) + random()) / float(img.height());
	// 			}
	// 			else{
	// 				ndcx = (float(i) + 0.5f) / float(img.width());
	// 				ndcy = (float(j) + 0.5f) / float(img.height());
	// 			}

	// 			//getting the screen space coordinates [-1, 1]
	// 			float sscx = (ndcx * 2.0f) - 1.0f;
	// 			float sscy = (ndcy * 2.0f) - 1.0f;

	// 			//generating the ray from the camera
	// 			Ray ray = cam->getPrimaryRay(sscx, -sscy);
	// 			 //std::cout<< accumulator.r<<std::endl;
	// 			accumulator = accumulator + integrator->getRadiance(ray);
	// 		}

	// 		img(i,j) = accumulator/(float)this->samples;
	// 	}
	// }
}

}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt {

void Renderer::test_render1(Image& img) {
	for (uint i = 0; i < img.width(); i++) {
		for (uint j = 0; j < img.height(); j++) {
			img(i,j) = a1computeColor(i,j,img.width(), img.height());
		}
	}
}
}

rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {
	for (uint i = 0; i < img.width(); i++) {
		for (uint j = 0; j < img.height(); j++) {
			
			//getting the normalized device coords [0, 1]
			float ndcx = (float(i) + 0.5f) / float(img.width());
			float ndcy = (float(j) + 0.5f) / float(img.height());

			//getting the screen space coordinates [-1, 1]
			float sscx = (ndcx * 2.0f) - 1.0f;
			float sscy = (ndcy * 2.0f) - 1.0f;

			Ray ray = cam->getPrimaryRay(sscx, -sscy);
			img(i, j) = a2computeColor(ray);
		}
	}
}

}
