#include "instancecanvas.h"
#include <QApplication>
#include <QJsonArray>

InstanceCanvas::InstanceCanvas(Canvas * design, KeyListWidget * list) : design(design), keyList(list) {
    connect(keyList->repo, &KeyRepository::keyDeleted, this, &InstanceCanvas::onKeyModelDeleted);
    connect(design, &Canvas::destroySpace, this, &InstanceCanvas::onSpaceDestroyed);
}

void InstanceCanvas::writeJson(QJsonObject &json) const
{
    QJsonObject instances;
    QJsonArray keyInstances;
    QJsonArray doorInstances;
    QJsonObject tokenInstance;

    // insert all the key instances
    for(auto key = keys.begin(); key != keys.end(); key++){
        keyInstances.append((*key)->toJson());
    }
    // insert all the door instances
    for(auto door = doors.begin(); door != doors.end(); door++){
        doorInstances.append((*door)->toJson());
    }
    // insert the startToken if there is one.
    // remember, the token can be null, in which case, we should indicate that in the json
    if(startToken != nullptr){
        QPointF tokenPoint = startToken->center();
        tokenInstance.insert("is-null", false);
        tokenInstance.insert("coords", QString(POINTFORMAT).arg(tokenPoint.x()).arg(tokenPoint.y()));
    } else {
        tokenInstance.insert("is-null", true);
    }
    // insert all the jsonObject wrappers into the main wrapper
    instances.insert(JSONINSTANCESKEY_MAXID, KeyInstance::ID);
    instances.insert(JSONINSTANCESKEYSKEY, keyInstances);
    instances.insert(JSONINSTANCESDOOR_MAXID, DoorInstance::ID);
    instances.insert(JSONINSTANCESDOORSKEY, doorInstances);
    instances.insert(JSONINSTANCESTARTTOKENKEY, tokenInstance);
    //finally, insert them all into the main json passed by argument
    json.insert(JSONINSTANCESKEY, instances);
}

void InstanceCanvas::readJson(const QJsonObject &json)
{
    int keysMaxId;
    int doorsMaxId;
    QList<KeyInstance*> keys;
    QList<DoorInstance*> doors;
    QRectF *startToken;
    if(!json.contains(JSONINSTANCESKEY) || !json[JSONINSTANCESKEY].isObject())
        return;
    // store the content in a QJsonObject to call less times the [] method
    QJsonObject jsonInstances = json[JSONINSTANCESKEY].toObject();

    // get the max id of the keys instances
    if(jsonInstances.contains(JSONINSTANCESKEY_MAXID) && jsonInstances[JSONINSTANCESKEY_MAXID].isDouble()){
        keysMaxId = jsonInstances[JSONINSTANCESKEY_MAXID].toInt();
    } else {
        throw std::runtime_error("no "+QString(JSONINSTANCESKEY_MAXID).toUtf8()+" key found in jsonObject");
    }

    // get the key instances
    if(jsonInstances.contains(JSONINSTANCESKEYSKEY) && jsonInstances[JSONINSTANCESKEYSKEY].isArray()){
        QJsonArray jsonArray = jsonInstances[JSONINSTANCESKEYSKEY].toArray();
        for(auto jsonKey = jsonArray.begin(); jsonKey != jsonArray.end(); jsonKey++){
            // Oof, I don't like the looks of that static method call (the keyList part looks iffy :( )
            if(jsonKey->isObject()) // no else so that if there is a non-obj entry, we don't freak out
                keys.append(new KeyInstance(KeyInstance::fromJson(jsonKey->toObject(), *keyList->repo)));
        }
    } else {
        throw std::runtime_error("no "+QString(JSONINSTANCESKEYSKEY).toUtf8()+" key found in jsonObject");
    }

    // get the max id of the doors instances
    if(jsonInstances.contains(JSONINSTANCESDOOR_MAXID) && jsonInstances[JSONINSTANCESDOOR_MAXID].isDouble()){
        doorsMaxId = jsonInstances[JSONINSTANCESDOOR_MAXID].toInt();
    } else {
        throw std::runtime_error("no "+QString(JSONINSTANCESDOOR_MAXID).toUtf8()+" key found in jsonObject");
    }

    // get the door instances
    if(jsonInstances.contains(JSONINSTANCESDOORSKEY) && jsonInstances[JSONINSTANCESDOORSKEY].isArray()){
        QJsonArray jsonArray = jsonInstances[JSONINSTANCESDOORSKEY].toArray();
        for(auto jsonDoor = jsonArray.begin(); jsonDoor != jsonArray.end(); jsonDoor++){
            if(jsonDoor->isObject()) // no else so that if there is a non-obj entry, we don't freak out
                doors.append(new DoorInstance(jsonDoor->toObject()));
        }
    } else {
        throw std::runtime_error("no "+QString(JSONINSTANCESDOORSKEY).toUtf8()+" key found in jsonObject");
    }

    if(jsonInstances.contains(JSONINSTANCESTARTTOKENKEY) && jsonInstances[JSONINSTANCESTARTTOKENKEY].isObject()){
        QJsonObject jsonObj = jsonInstances[JSONINSTANCESTARTTOKENKEY].toObject();
        if(jsonObj.contains("is-null") && jsonObj["is-null"].isBool()){
            if(jsonObj["is-null"].toBool()){ // if the start token is null => it was not set
                startToken = nullptr;
            } else {
                if(jsonObj.contains("coords") && jsonObj["coords"].isString()){
                    QPointF point = json::parsePoint(jsonObj["coords"].toString());
                    startToken = new QRectF(point.x()-8, point.y()-8, 16, 16);
                    TODO("if we end up making startToken an image instead of an ellipse, change this");
                } else {
                    throw std::runtime_error("no coords for start token");
                }
            }
        } else {
            throw std::runtime_error("malformed json for start token");
        }
    } else {
        throw std::runtime_error("no "+QString(JSONINSTANCESTARTTOKENKEY).toUtf8()+" key found in jsonObject");
    }

    this->keys.clear();
    this->doors.clear();

    this->keys = keys;
    KeyInstance::ID = keysMaxId;
    this->doors = doors;
    DoorInstance::ID = doorsMaxId;
    this->startToken = startToken;
}

void InstanceCanvas::showEvent(QShowEvent *)
{
    design->show();
}

void InstanceCanvas::paintEvent(QPaintEvent *event)
{
    design->update(); // we draw our design, then we'll drow aurselves
    QPainter painter(this);
    QPen pen;
    // setting the pen properties
    pen.setColor(QColor(0,50,255, 128));
    pen.setWidth(4);
    painter.setPen(pen); // green color
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBackgroundMode(Qt::BGMode::TransparentMode);
    painter.scale(design->grid.getScale(), design->grid.getScale());

    // drawing doors
    for(auto door = doors.begin(); door != doors.end(); door++){
        (*door)->drawSelf(painter, design->grid.getOffset());
        //painter.drawLine((*door)->translated(design->grid.getOffset()));
        //painter.drawPolygon((*door)->area.translated(design->grid.getOffset()));
    }

    // drawing keys
    for(auto key = keys.begin(); key != keys.end(); key++){
        painter.drawImage((*key)->translated(design->grid.getOffset()),QImage("../key.png")); // I hope this does not blow up
        // TODO draw different images whether the key is power up or not
    }

    // drawing the start point
    if(startToken != nullptr){
        // TODO maybe replace the ellipse of the token with an image
        painter.drawEllipse(startToken->translated(design->grid.getOffset()));
        // now we draw the error path if there is one
        if(path.length() > 0){
            pen.setColor(QColor(255,0,0,128));
            pen.setWidth(2);
            painter.setPen(pen);
            QPainterPath errorPath(startToken->center());
            for(auto item = path.begin(); item != path.end(); item++){
                errorPath.lineTo((*item)->instancePosition());
                //errorPath.addPolygon((*item)->boundPolygon());
                //errorPath.lineTo((*item)->instancePosition());
            }
            painter.drawPath(errorPath.translated(design->grid.getOffset()));
        }
    }

    // drawing bounding rectangle of the selected instance to highlight it
    if(selected != nullptr){
        pen.setColor(QColor(255,255,0,128));
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawPolygon(selected->boundPolygon().translated(design->grid.getOffset()));
    }
}

void InstanceCanvas::mousePressEvent(QMouseEvent *event)
{
    //path.clear();
    design->grid.mousePressEventHandler(event); // tell the grid

    if(event->buttons() & Qt::LeftButton){ // if it was a left click, creation will happen
        if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)){ // if ctrl is being held
            QPointF point = design->grid.centerOfCellAt(event->pos());
            if(isPlaceEmptyForToken(point)) // if the token is not already in that place
                createStartPoint(point); // create the starting point
        } else if(QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){ // if shift is being held
            QPointF *point = new QPointF(design->grid.nearestPoint(event->pos()));
            startCreatingDoor(point); // start the creation of a door
        } else { // if no keyboard key is being held
            // TODO check if an already existing element was clicked and then show their info in console!!!
            KeyInstance * key = keyAt(design->grid.adapted(event->pos())); // try to find a key at specific cursor point
            DoorInstance * door = doorAt(design->grid.adapted(event->pos())); // try to find a door at specific cursor point
            if(key != nullptr || door != nullptr){ // if there is a selection
                if(key != nullptr){ // if there is a key in the selected point
                    selected = key;
                }
                else if(door != nullptr){ // if there is a door in the selected point
                    selected = door;
                    emit selectDoor(*door);
                }
                //DELETEME {
                path.append(selected);
                update();
                //DELETEME }
                emit select(*selected);
            } else { // if there is no selection, then we create a key
                QPointF point = design->grid.centerOfCellAt(event->pos());
                createKeyInstance(point); // create a key
            }
        }
    } else if(event->button() & Qt::RightButton){ // if it was a right click, destruction will happen
        path.clear(); // DELETEME
        QPointF point = design->grid.adapted(event->pos()); // we adapt the event position for grid transformations
        destroyAt(&point); // and destroy all elements under said point
    }
    update(); // update the UI for changes to be applied
}

void InstanceCanvas::mouseMoveEvent(QMouseEvent *event)
{
    design->grid.mouseMoveEventHandler(event);
}

void InstanceCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF end(design->grid.nearestPoint(event->pos()));
    design->grid.mouseReleaseEventHandler(event);
    // some stackOverflow wizard told me that != works as xor for bool values as follows:
    // A xor B <=> !A != !B
    // the more you know

    // check if: start point exists (lol)
    // if its 1st and 2nd coords are different
    // if it is a horizontal or vertical line (this plus the above, is checked with the xor mentioned before)
    if(start != nullptr &&
            (!(std::abs(start->x()-end.x()) < UMBRAL) != !(std::abs(start->y()-end.y()) < UMBRAL))){
        QLineF line(*start, end);
        doors.append(new DoorInstance(line));
        selected = doors.last();
        emit selectDoor(*doors.last());
        emit select(*selected);
    }
    delete start;
    start = nullptr;
}

void InstanceCanvas::wheelEvent(QWheelEvent *event)
{
    design->grid.wheelEvent(event);
}

void InstanceCanvas::createStartPoint(QPointF &point)
{
    if(!design->contains(point))
        return;
    delete startToken;
    startToken = new QRectF(point.x()-8, point.y()-8, 16, 16); // numeric values are for visualization dimensions
}

void InstanceCanvas::startCreatingDoor(QPointF *point)
{
    start = point;
}

void InstanceCanvas::createKeyInstance(QPointF &point)
{
    if(!design->contains(point))
        return;
    const Key &model = keyList->selectedKey();
    if(model.isValid()){ // if there was indeed a selected key
        keys.append(new KeyInstance(model, point.x(), point.y()));
        selected = keys.last();
        emit select(*selected);
    }
}

void InstanceCanvas::destroyAt(const QPointF *point)
{
    /// note: we could return after the first destruction, since, in construction, we don't allow
    /// multiple objects overlapping, but we leave it this way since it's stronger, more secure
    /// and more intuitive, just in case things don't go as planned
    // destroy startToken if it is under the point
    if(startToken != nullptr && startToken->contains(*point)){
        delete startToken;
        startToken = nullptr;
    }
    // destroy key if it is under the point
    for(int i = keys.size()-1; i >= 0; i--){
        if(keys[i]->contains(*point)){
            if(selected == keys[i]){
                selected = nullptr;
                emit clearSelection();
            }
            delete keys[i];
            keys.removeAt(i);
        }
    }
    // destroy door if it is under the point
    for(int i = doors.size()-1; i >= 0; i--){
        if(doors[i]->contains(*point)){
            if(selected == doors[i]){
                selected = nullptr;
                emit clearSelection();
            }
            delete doors[i];
            doors.removeAt(i);
        }
    }
}

bool InstanceCanvas::isPlaceEmptyForKey(const QPointF &point)
{
    // loop through all keys
    for(auto key = keys.begin(); key != keys.end(); key++)
        if((*key)->contains(point)) // if one key's area contains the point, then no, the place is not empty
            return false;
    return true; // if no key's area contains the point, then yes, the place is empty
}

KeyInstance *InstanceCanvas::keyAt(const QPointF &point)
{
    for(auto key = keys.begin(); key != keys.end(); key++)
        if((*key)->contains(point)) // if one key's area contains the point, then no, the place is not empty
            return *key;
    return nullptr; // if no key's area contains the point, then yes, the place is empty
}

DoorInstance *InstanceCanvas::doorAt(const QPointF &point)
{
    for(auto door = doors.begin(); door != doors.end(); door++)
        if((*door)->contains(point)) // if one key's area contains the point, then no, the place is not empty
            return *door;
    return nullptr; // if no key's area contains the point, then yes, the place is empty
}

bool InstanceCanvas::isPlaceEmptyForToken(const QPointF &point)
{
    return startToken == nullptr || startToken->contains(point);
}

void InstanceCanvas::onKeyModelDeleted(const Key &model)
{
    for(int i = keys.size()-1; i >= 0; i--){
        if(keys[i]->model == model){
            delete keys[i];
            keys.removeAt(i);
        }
    }
}

void InstanceCanvas::onSpaceDestroyed(const QPolygonF &poly)
{
    for(int i = keys.size()-1; i >= 0; i--){
        if(poly.containsPoint(keys[i]->center(), Qt::FillRule::OddEvenFill)){
            delete keys[i];
            keys.removeAt(i);
        }
    }
    if(startToken != nullptr && poly.containsPoint(startToken->center(), Qt::FillRule::OddEvenFill)){
        delete startToken;
        startToken = nullptr;
    }
}
