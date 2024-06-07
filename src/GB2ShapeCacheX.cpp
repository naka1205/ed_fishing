#include "GB2ShapeCacheX.h"
#include "box2dx.h"


static GB2ShapeCache *_sharedGB2ShapeCache = NULL;

GB2ShapeCache* GB2ShapeCache::sharedGB2ShapeCache(void) {
	if (!_sharedGB2ShapeCache) {
		_sharedGB2ShapeCache = new GB2ShapeCache();
        _sharedGB2ShapeCache->init();
	}
	
	return _sharedGB2ShapeCache;
}
GB2ShapeCache* GB2ShapeCache::getInstance(void)
{
	return GB2ShapeCache::sharedGB2ShapeCache();
}
void GB2ShapeCache::purge()
{
	if(_sharedGB2ShapeCache)
	{
		this->reset();
		delete _sharedGB2ShapeCache;
	}
}
bool GB2ShapeCache::init() {
	return true;
}

void GB2ShapeCache::reset() {
	std::map<std::string, BodyDef *>::iterator iter;
	for (iter = shapeObjects.begin() ; iter != shapeObjects.end() ; ++iter) {
		delete iter->second;
	}
	shapeObjects.clear();
}

void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape) {
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());
	
	BodyDef *so = (*pos).second;
    
	FixtureDef *fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}

FixtureDef*GB2ShapeCache::getFixtureDefsForName(const std::string& shape)
{
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());
	
	BodyDef *so = (*pos).second;
    
	FixtureDef *fix = so->fixtures;

	return fix;
}

Point GB2ShapeCache::anchorPointForShape(const std::string &shape) {
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());
	
	BodyDef *bd = (*pos).second;
	return bd->anchorPoint;
}


void GB2ShapeCache::addShapesWithFile(const std::string &plist) {
    
	//const char *fullName = CCFileUtils::sharedFileUtils()->fullPathForFilename(plist.c_str()).c_str();
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile((plist.c_str()));
    // not triggered - cocos2dx delivers empty dict if non was found
    
	//CCAssert(dict != NULL, "Shape-file not found");
    
    //CCAssert(dict.empty(), "plist file empty or not existing");
	
	ValueMap metadataDict = dict.at("metadata").asValueMap();
    
    //int format = metadataDict.at("format").asInt();
    ptmRatio = metadataDict.at("ptm_ratio").asFloat();
    printf("ptmRatio = %f",ptmRatio);
	//CCAssert(format == 1, "Format not supported");
    
    
	ValueMap bodyDict = dict.at("bodies").asValueMap();
    
    b2Vec2 vertices[b2_maxPolygonVertices];

    std::string bodyName;
	ValueMap bodyData;
    //iterate body list
    for(auto dictElem : bodyDict)
    //CCDICT_FOREACH(bodyDict,dictElem )
    {
        bodyData = dictElem.second.asValueMap();
        bodyName = dictElem.first;
        
        BodyDef *bodyDef = new BodyDef();
        bodyDef->anchorPoint = PointFromString(bodyData.at("anchorpoint").asString());
        ValueVector fixtureList = bodyData.at("fixtures").asValueVector();
        FixtureDef **nextFixtureDef = &(bodyDef->fixtures);
        
        //iterate fixture list
        //Ref *arrayElem;
        for(auto arrayElem : fixtureList)
        //CCARRAY_FOREACH(fixtureList, arrayElem)
        {
            b2FixtureDef basicData;
            ValueMap fixtureData = arrayElem.asValueMap();
            
            basicData.filter.categoryBits = fixtureData.at("filter_categoryBits").asInt();
            
            basicData.filter.maskBits = fixtureData.at("filter_maskBits").asInt();
            basicData.filter.groupIndex = fixtureData.at("filter_groupIndex").asInt();
            basicData.friction = fixtureData.at("friction").asFloat();
            
            basicData.density = fixtureData.at("density").asFloat();
            
            basicData.restitution = fixtureData.at("restitution").asFloat();
            //CCLog("%s", static_cast<__String *>(fixtureData->objectForKey("id"))->getCString());
            //basicData.userData = __String::create(static_cast<__String *>(fixtureData->objectForKey("id"))->getCString())->retain();
            
            basicData.isSensor = fixtureData.at("isSensor").asBool();
            int callbackData = 0;
//            if (!fixtureData.at("userdataCbValue").isNull()) {
//                string cb = fixtureData.at("userdataCbValue").asString();
//                
//                if (cb != "")
//                    callbackData =fixtureData.at("userdataCbValue").asInt();
//            }
            
            
            
			std::string fixtureType = fixtureData.at("fixture_type").asString();
            
			if (fixtureType == "POLYGON") 
			{
                ValueVector polygonsArray = fixtureData.at("polygons").asValueVector();
				
                //Ref *dicArrayElem;
                for(auto dicArrayElem : polygonsArray)
                //CCARRAY_FOREACH(polygonsArray, dicArrayElem)
                {
                    FixtureDef *fix = new FixtureDef();
                    fix->fixture = basicData; // copy basic data
                    fix->callbackData = callbackData;
                    
                    b2PolygonShape *polyshape = new b2PolygonShape();
                    int vindex = 0;
                    
                    ValueVector polygonArray = dicArrayElem.asValueVector();
                    
                    assert(polygonArray.capacity() <= b2_maxPolygonVertices);
                    
                    //Ref *piter;
                    for(auto piter : polygonArray)
                    //CCARRAY_FOREACH(polygonArray, piter)
                    {
                        string verStr = piter.asString();
                        Point offset = PointFromString(verStr);
                        vertices[vindex].x = (offset.x / ptmRatio) ;
                        vertices[vindex].y = (offset.y / ptmRatio) ;
                        vindex++;
                    }
                    
                    polyshape->Set(vertices, vindex);
                    fix->fixture.shape = polyshape;
                    
                    // create a list
                    *nextFixtureDef = fix;
                    nextFixtureDef = &(fix->next);
                }
                
                
			}
            else if (fixtureType == "CIRCLE") 
			{
				FixtureDef *fix = new FixtureDef();
                fix->fixture = basicData; // copy basic data
                fix->callbackData = callbackData;
                
                ValueMap circleData = fixtureData.at("circle").asValueMap();
                
                b2CircleShape *circleShape = new b2CircleShape();
				
                circleShape->m_radius = circleData.at("radius").asFloat() / ptmRatio;
				Point p = PointFromString(circleData.at("position").asString());
                circleShape->m_p = b2Vec2(p.x / ptmRatio, p.y / ptmRatio);
                fix->fixture.shape = circleShape;
				
                // create a list
                *nextFixtureDef = fix;
                nextFixtureDef = &(fix->next);
                
			}
            else {
				SDL_assert(0);
			}
		}
        // add the body element to the hash
        shapeObjects[bodyName] = bodyDef;
        
    }
    
}