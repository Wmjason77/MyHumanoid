#include "animorph/PoseTranslation.h"

#include <cstdio>
#include "Logger.h"

#define FF_VERTEX_N 10


using std::multiset;

namespace Animorph
{

PoseTranslation::PoseTranslation()
        : target(new Target())
        , formFactor(1.0, 1.0, 1.0)
        , minAngle(0.0f)
        , maxAngle(0.0f)
        , normalize(false)
        ,
        // inFilename(),
        cat()
{
}

bool PoseTranslation::load(const std::string & filename)
{
	char        tmp[MAX_LINE_BUFFER];
	vector<int> tmpVector();
	int         ret = 0;

	bool rc = true; // assume "success" by default

	// get the current locale
	char * locale = ::setlocale(LC_NUMERIC, NULL);

	// set it to "C"-Style ( the . (dot) means the decimal marker for floats)
	::setlocale(LC_NUMERIC, "C");

	// info file
	std::string infoFileName = filename + ".info";
	log_debug("Open File: {}", infoFileName);
	FILE * fd = fopen(infoFileName.c_str(), "r");

	if(fd == NULL)
		return false;

	// form factor
	fgets(tmp, MAX_LINE_BUFFER, fd);
	if(tmp == NULL) // end of file reached?
		return false;

	ret = sscanf(tmp, "%f,%f,%f", &originalSize[0], &originalSize[1], &originalSize[2]);

	if(ret == EOF) // end of file reached?
		return false;

	fgets(tmp, MAX_LINE_BUFFER, fd);
	if(tmp == NULL) // end of file reached?
		return false;

	ret = sscanf(tmp, "%f,%f", &minAngle, &maxAngle);

	if(ret == EOF) // end of file reached?
		return false;

	fclose(fd);

	// reset locale after file was written
	::setlocale(LC_NUMERIC, locale);

	if(!target->load(filename))
		return false;

	return rc;
}

void PoseTranslation::calcFormFactor(const VertexVector & vertexvector)
{
	multiset<float> minXSet, maxXSet;
	multiset<float> minYSet, maxYSet;
	multiset<float> minZSet, maxZSet;

	int   counter  = 0;
	int   n_vertex = FF_VERTEX_N;
	float minX = 0, maxX = 0;
	float minY = 0, maxY = 0;
	float minZ = 0, maxZ = 0;
	// pair<set<float>::iterator, bool> pr;

	Target & tmpTarget = getTarget();

	if(tmpTarget.size() < (FF_VERTEX_N * 2)) {
		n_vertex = (int)(tmpTarget.size() / 2);
	}

	for(Target::const_iterator target_it = tmpTarget.begin(); target_it != tmpTarget.end();
	    target_it++) {
		const TargetData & td(*target_it);
		if(counter < n_vertex) {
			minXSet.insert(vertexvector[td.vertex_number].pos.x);
			maxXSet.insert(vertexvector[td.vertex_number].pos.x);

			minYSet.insert(vertexvector[td.vertex_number].pos.y);
			maxYSet.insert(vertexvector[td.vertex_number].pos.y);

			minZSet.insert(vertexvector[td.vertex_number].pos.z);
			maxZSet.insert(vertexvector[td.vertex_number].pos.z);

			counter++;
		} else {
			if(vertexvector[td.vertex_number].pos.x < *(--minXSet.end())) {
				minXSet.insert(vertexvector[td.vertex_number].pos.x);
				minXSet.erase(--(minXSet.end()));
			}
			if(vertexvector[td.vertex_number].pos.x > *(maxXSet.begin())) {
				maxXSet.insert(vertexvector[td.vertex_number].pos.x);
				maxXSet.erase(maxXSet.begin());
			}

			if(vertexvector[td.vertex_number].pos.y < *(--minYSet.end())) {
				minYSet.insert(vertexvector[td.vertex_number].pos.y);
				minYSet.erase(--(minYSet.end()));
			}
			if(vertexvector[td.vertex_number].pos.y > *(maxYSet.begin())) {
				maxYSet.insert(vertexvector[td.vertex_number].pos.y);
				maxYSet.erase(maxYSet.begin());
			}

			if(vertexvector[td.vertex_number].pos.z < *(--minZSet.end())) {
				minZSet.insert(vertexvector[td.vertex_number].pos.z);
				minZSet.erase(--(minZSet.end()));
			}
			if(vertexvector[td.vertex_number].pos.z > *(maxZSet.begin())) {
				maxZSet.insert(vertexvector[td.vertex_number].pos.z);
				maxZSet.erase(maxZSet.begin());
			}
		}
	}

	for(const auto & it : minXSet) {
		minX += it;
	}
	for(const auto & it : maxXSet) {
		maxX += it;
	}

	for(const auto & it : minYSet) {
		minY += it;
	}
	for(const auto & it : maxYSet) {
		maxY += it;
	}

	for(const auto & it : minZSet) {
		minZ += it;
	}
	for(const auto & it : maxZSet) {
		maxZ += it;
	}

	int xsize = maxXSet.size();
	int ysize = maxYSet.size();
	int zsize = maxZSet.size();

	formFactor = glm::vec3((maxX / xsize - minX / xsize) / originalSize[0],
	                       (maxY / ysize - minY / ysize) / originalSize[1],
	                       (maxZ / zsize - minZ / zsize) / originalSize[2]);
}

}
