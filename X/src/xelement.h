#pragma once

#include <vector>
#include <set>
#include <memory>
#include <string>


class XElement : public std::enable_shared_from_this<XElement>
{
public:
    enum Direction
    {
        Forward,
        Backward
    };

    typedef std::shared_ptr<XElement> SPXElement;
    typedef std::vector<char> Data;
    typedef std::shared_ptr<std::vector<char> > SPData;

    virtual std::string name() = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    // 双向的数据输入.
    virtual void pushForward( SPData data ) = 0;
    virtual void pushBackward( SPData data ) = 0;

    // pipeline的上下游管理.
    virtual void addElement( Direction direction, SPXElement element );
    virtual void removeElement( Direction direction, SPXElement element );
    

    // 错误
    struct Error
    {
        Direction direction;
        SPXElement errorElement;
        SPXElement previousElement;
        int errorCode;
        std::string description;
    };
    typedef std::shared_ptr<Error> SPError;
    virtual void onError( SPError err ) = 0;

protected:
    // 向pipeline的上下游发送数据.
    virtual void sendDataForwardNext( SPData data );
    virtual void sendDataBackwardNext( SPData data );

    virtual void sendErrorNext( SPError err );

    std::set<SPXElement> sources;
    std::set<SPXElement> sinks;
};