//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include "Triangle.h"
#include "animation/Animatable.h"
#include <SFML/Graphics.hpp>
#include "Object.h"

class Mesh : public Object, public Animatable {
private:
    std::vector<Triangle> _tris;
    sf::Color _color = sf::Color(255, 245, 194);
    bool _visible = true;

    Mesh& operator*=(const Matrix4x4& matrix4X4);
public:
    Mesh() = default;
    Mesh(const Mesh& mesh);

    explicit Mesh(const std::vector<Triangle>& tries);
    Mesh& operator=(const Mesh& mesh);
    explicit Mesh(const std::string& filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    Mesh& loadObj(const std::string& filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    [[nodiscard]] std::vector<Triangle>const &triangles() const { return _tris; }
    [[nodiscard]] std::vector<Triangle>& triangles() override { return _tris; }
    void setTriangles(const std::vector<Triangle>& t) override { _tris = t; }

    // Translate body
    void translate(const Point4D& t) override;
    void translateToPoint(const Point4D& point);
    // Rotate body around XYZ axes
    void rotate(const Point4D& r) override;
    // Rotate body around normalised vector 'v' by 'r' radians
    void rotate(const Point4D& v, double r) override;
    // Rotate body around XYZ by (r.x, r.y, r.z) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& r) override;
    // Rotate body around normalised vector 'v' by 'r' radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& v, double r) override;
    void scale(const Point4D& s);

    void rotateToAngle(const Point4D& v) { rotate(v - _angle); }

    [[nodiscard]] Point4D position() const override { return _position; }
    [[nodiscard]] Point4D angle() const override { return _angle; }

    [[nodiscard]] sf::Color color() const override { return _color; }
    void setColor(sf::Color c) override;

    Mesh static Obj(const std::string& filename);
    Mesh static LineTo(const Point4D& from, const Point4D& to, double line_width = 0.1, sf::Color color = {150, 150, 150, 255});

    void setVisible(bool visibility) { _visible = visibility; }
    [[nodiscard]] bool isVisible() const { return _visible; }

    std::vector<std::shared_ptr<Mesh>> static LoadObjects(const std::string& filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});
};


#endif //INC_3DZAVR_MESH_H
